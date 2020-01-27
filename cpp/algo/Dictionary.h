
struct DictEntry
{
    unsigned char zone : 1;
    unsigned char keyLen : 7;
    unsigned int hash;
    unsigned long key;

};

class Dictionary
{
    private:
        DictEntry* tbl;
        int num_buckets;
        int num_entries;
        int max_num_entries;

};