#ifndef _PersistentLeafNode_
#define _PersistentLeafNode_

#include <string>
#include <libpmemobj++/make_persistent.hpp>
#include <libpmemobj++/make_persistent_array.hpp>
#include <libpmemobj++/persistent_ptr.hpp>
#include <libpmemobj++/transaction.hpp>
#include <libpmemobj++/pool.hpp>
#include <libpmemobj/pool_base.h>

using pmem::obj::delete_persistent;
using pmem::obj::make_persistent;
using pmem::obj::persistent_ptr;
using pmem::obj::pool;
using pmem::obj::transaction;

struct PersistentLeaf {
    persistent_ptr<PersistentLeaf> right;
    persistent_ptr<char[]>         keyValue; 

    void setKeySizeDirect(char *p, uint32_t v) {
		    *((uint32_t *)(p)) = v;
	  }

	  void setValueSizeDirect(char *p, uint32_t v) {
		    *((uint32_t *)((char *)(p) + sizeof(uint32_t))) = v;
	  }

    void put(const string &key, const string &value) {
        if (keyValue) {
          char* p = keyValue.get();
          delete_persistent<char[]>(keyValue, sizeof(uint32_t) + sizeof(uint32_t) + keySizeDirect(p) + valueSizeDirect(p) + 2);
        }
        size_t ksize = key.size();
        size_t vsize = value.size();
        size_t size  = ksize + vsize + 2 + sizeof(uint32_t) + sizeof(uint32_t);

        keyValue = make_persistent<char[]>(size);

        char *p = keyValue.get();
        setKeySizeDirect(p,   (uint32_t)ksize);
        setValueSizeDirect(p, (uint32_t)vsize);
        
        char *kvptr = p + sizeof(uint32_t) + sizeof(uint32_t);
        memcpy(kvptr, key.data(), ksize);   
        kvptr += ksize + 1;		              
        memcpy(kvptr, value.data(), vsize); 
    }

    const char *key() const {
        return ((char *)(keyValue.get()) + sizeof(uint32_t) + sizeof(uint32_t));
    }

    const char *value() const {
        return ((char *)(keyValue.get()) + sizeof(uint32_t) + sizeof(uint32_t) + keySize() + 1);
    }

    uint32_t keySize() const {
        return *((uint32_t *)(keyValue.get()));
    }

    uint32_t valueSize() const {
        return *((uint32_t *)((char *)(keyValue.get()) + sizeof(uint32_t)));
    }

    uint32_t keySizeDirect(char *p) {
		    return *((uint32_t *)(p));
	  }
    
	  uint32_t valueSizeDirect(char *p) {
		   return *((uint32_t *)((char *)(p) + sizeof(uint32_t)));
	  }
};

#endif