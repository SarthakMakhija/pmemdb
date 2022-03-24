#ifndef _Status_
#define _Status_

namespace pmem {
    namespace storage {
        namespace internal {
            enum Status {
                Ok,
                Failed,
                KeyNotFound,
                KeyAlreadyExists
            };
        }
    }
}
#endif