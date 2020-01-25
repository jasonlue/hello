package io.github.jasonlue.hello;

import org.hibernate.engine.jdbc.connections.internal.DatasourceConnectionProviderImpl;

public class DataSourceConnectionProvider extends DatasourceConnectionProviderImpl {
    public DataSourceConnectionProvider() {
        setDataSource(HibernateFactory.INSTANCE.getDataSource());
    }
}