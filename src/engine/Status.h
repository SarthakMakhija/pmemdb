#ifndef _Status_
#define _Status_

namespace pmem {
    namespace storage {
        enum Status {
            Ok,
            Failed,
            KeyNotFound,
            KeyAlreadyExists
        };
    }
}
#endif