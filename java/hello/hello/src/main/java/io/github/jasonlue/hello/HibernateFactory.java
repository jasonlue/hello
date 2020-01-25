package io.github.jasonlue.hello;

import java.beans.PropertyVetoException;
import java.sql.Timestamp;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.function.Consumer;
import java.util.function.Function;

import javax.sql.DataSource;

import com.mchange.v2.c3p0.ComboPooledDataSource;

import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.boot.Metadata;
import org.hibernate.boot.MetadataSources;
import org.hibernate.boot.registry.StandardServiceRegistry;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

enum HibernateFactory {// https://www.tutorialspoint.com/hibernate/hibernate_configuration.htm
    INSTANCE;

    Logger logger = LoggerFactory.getLogger(HibernateFactory.class);
    private final DataSource dataSource;
    private SessionFactory sessionFactory;

    public DataSource getDataSource() {
        return dataSource;
    }

    HibernateFactory() {
        dataSource = createDataSource();
        // createSessionFactory() will loop back to access dataSource through
        // SessionConnectionProvide
        // basically have to init it out of constructor
        sessionFactory = null;// createSessionFactory();
    }

    private DataSource createDataSource() {
        try {
            ComboPooledDataSource ds = new ComboPooledDataSource();
            ds.setDataSourceName("main");
            ds.setDriverClass("org.postgresql.Driver");
            ds.setJdbcUrl("jdbc:postgresql://localhost:5432/sensor");
            ds.setUser("sensor");
            ds.setPassword("sensor");
            ds.setInitialPoolSize(3);
            ds.setMinPoolSize(1);
            ds.setAcquireIncrement(1);
            ds.setMaxPoolSize(6);

            return ds;
        } catch (PropertyVetoException e) {
            throw new RuntimeException(e);
        }
    }

    private SessionFactory createSessionFactory() { // Configuration -> ServiceRegistry ->Metadata ->SessionFactory ->
                                                    // Session -> Transaction -> Query
        Configuration cfg = new Configuration().configure("hibernate.cfg.xml");
        // load settings from hibernate.cfg.xml. empty.
        // set acutal configurations here. to load setting from one config file.
        // hookup external datasource.
        cfg.setProperty("hibernate.connection.provider_class", "io.github.jasonlue.hello.DataSourceConnectionProvider");
        cfg.setProperty("hibernate.current_session_context_class", "thread");
        cfg.setProperty("hibernate.show_sql", "true");
        cfg.setProperty("hibernate.dialect", "org.hibernate.dialect.PostgreSQLDialect");
        cfg.setProperty("hibernate.jdbc.batch_size", "10");

        StandardServiceRegistry reg = new StandardServiceRegistryBuilder().applySettings(cfg.getProperties()).build();
        Metadata md = new MetadataSources(reg).addAnnotatedClass(Sensor.class).buildMetadata();
        return md.buildSessionFactory();
    }

    public void updateTransaction(Consumer<Session> function) {
        if (sessionFactory == null) {
            sessionFactory = createSessionFactory();
        }
        Transaction tx = null;
        Session session = null;
        try {
            session = sessionFactory.openSession();
            tx = session.beginTransaction();
            function.accept(session);
            tx.commit();
        } catch (HibernateException e) {
            if (tx != null) {
                tx.rollback();
                e.printStackTrace();
            }
        } finally {
            if (session != null) {
                session.close();
            }
        }
    }

    public <R> R queryTransaction(Function<Session, R> function) {
        if (sessionFactory == null) {
            sessionFactory = createSessionFactory();
        }
        Session session = null;
        Transaction tx = null;
        try {
            session = sessionFactory.openSession();
            tx = session.beginTransaction();
            R r = function.apply(session);
            tx.commit();
            return r;
        } catch (HibernateException e) {
            e.printStackTrace();
            if (tx != null) {
                tx.rollback();
            }
        } finally {
            if (session != null)
                session.close();
        }
        return null;
    }

    // http://docs.jboss.org/hibernate/orm/3.5/reference/en/html/batch.html
    public static void sqlDeleteAll() {// raw SQL format
        HibernateFactory.INSTANCE
                .updateTransaction(session -> session.createSQLQuery("delete from sensor").executeUpdate());
    }

    public static void sqlInsert() {
        HibernateFactory.INSTANCE.updateTransaction(session -> {
            for (int i = 0; i < 10; i++) {
                String sqlInsert = "insert into sensor (sensor_id, account_code, created, updated) values("
                        + "'sensor_id_" + Integer.toString(i) + "','account_code_" + Integer.toString(i) + "','"
                        + new Timestamp(new Date().getTime()).toString() + "','"
                        + new Timestamp(new Date().getTime()).toString() + "')";
                int rowsAffected = session.createSQLQuery(sqlInsert).executeUpdate();
                System.out.println("rows affected: " + rowsAffected);
            }
        });
    }

    public static void hqlDeleteAll() {
        HibernateFactory.INSTANCE.updateTransaction(session -> session.createQuery("delete Sensor").executeUpdate());
    }

    public static void sqlList() {// BUG
        List sensors = HibernateFactory.INSTANCE // Pure SQL use table isntead of object.
                .queryTransaction(session -> session.createSQLQuery("select * from Sensor").list());
        for (Iterator it = sensors.iterator(); it.hasNext();) {
            Sensor s = (Sensor) it.next();// the item is not Sensor?? crashed.
            System.out.println(it.next());
        }
    }

    public static void hqlList() {
        // Because HQL works on persistent objects. Sensor is the class name, so
        // it uses the class name instead of table name.
        List sensors = HibernateFactory.INSTANCE // HQL uses object: Sensor. cannot use *
                .queryTransaction(session -> session.createQuery("from Sensor").list());
        for (Iterator it = sensors.iterator(); it.hasNext();) {
            Sensor s = (Sensor) it.next();
            System.out.println(s);
        }
    }

    public static void hqlInsert() { // HQL doesn't support insert with values!!!
        HibernateFactory.INSTANCE.updateTransaction(session -> {
            for (int i = 0; i < 10; i++) {
                String hqlInsert = "insert into Sensor (sensorId, accountCode, created, updated) values(:sensorId, :accountCode, :created, :updated)";
                int rowsAffected = session.createQuery(hqlInsert)
                        .setString("sensorId", "sensor_id_" + Integer.toString(i))
                        .setString("accountCode", "account_code_" + Integer.toString(i)).executeUpdate();
                System.out.println("rows affected: " + rowsAffected);
            }
        });
    }

    public static void hqlUpdate() {// make sure hql all objects and attributes. no database concepts, not event
                                    // schema qualifier. also, getter/setting attributes starts lower case.
        HibernateFactory.INSTANCE.updateTransaction(session -> {
            String hql = "update Sensor set accountCode = :accountCode where sensorId = :sensorId";
            Query q = session.createQuery(hql);
            q.setString("accountCode", "account_code_66");
            q.setString("sensorId", "sensor_id_6");
            int rowsAffected = q.executeUpdate();
            System.out.println("rows affected: " + rowsAffected);
        });
    }

    public static void insert() {// orm way, the usual way. every newly inserted row gets into the cache.
        HibernateFactory.INSTANCE.updateTransaction(session -> {
            for (int i = 0; i < 100; i++) {
                Sensor s = new Sensor("sensor_id_" + Integer.toString(i), "account_code_" + Integer.toString(i));
                session.save(s);
            }
        });
    }

    public static void batchInsert() {// flush() put the cache to physical and clear() clears the cache.
        HibernateFactory.INSTANCE.updateTransaction(session -> {
            for (int i = 0; i < 100; i++) {
                Sensor s = new Sensor("sensor_id_" + Integer.toString(i), "account_code_" + Integer.toString(i));
                session.save(s);
                if (i % 10 == 0) {// handle the orm cache, make it the same as hibernate.jdbc.batch_size.
                    session.flush();
                    session.clear();
                }
            }
        });
    }

    public static void update() {// update
        String sensorId = "sensor_id_6";
        String accountCode = "new_account_code_6";
        HibernateFactory.INSTANCE.updateTransaction(session -> {
            Sensor s = (Sensor) session.get(Sensor.class, sensorId);
            s.setAccountCode(accountCode);
            session.update(s);
        });
    }

    public static void delete(String sensorId) {// delete no map. what about the cache?
        HibernateFactory.INSTANCE.updateTransaction(session -> {
            Sensor s = (Sensor) session.get(Sensor.class, sensorId);
            session.delete(s);
        });
    }

    public static void main(String[] args) {
        // sqlList(); bug to be investigated.
        hqlList();
        sqlDeleteAll();
        // hqlDeleteAll();
        hqlList();
        sqlInsert();
        hqlUpdate();
        // insert();
        // batchInsert();
        hqlList();

    }

}