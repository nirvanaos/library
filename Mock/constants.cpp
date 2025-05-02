/*
* Nirvana mock library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#include "constants.h"
#include <posix_defs.h>
#include <errors.h>

#define FLAG(n) Flag const_##n = n

namespace Nirvana {
namespace Test {

typedef const unsigned int Flag;

FLAG (O_WRONLY);
FLAG (O_RDWR);
FLAG (O_CREAT);
FLAG (O_EXCL);
FLAG (O_TRUNC);
FLAG (O_APPEND);
FLAG (O_NONBLOCK);
FLAG (O_DIRECT);
FLAG (O_TEXT);
FLAG (O_SYNC);

FLAG (S_IRUSR);
FLAG (S_IWUSR);
FLAG (S_IXUSR);
FLAG (S_IRGRP);
FLAG (S_IWGRP);
FLAG (S_IXGRP);
FLAG (S_IROTH);
FLAG (S_IWOTH);
FLAG (S_IXOTH);

FLAG (S_IFIFO);
FLAG (S_IFCHR);
FLAG (S_IFDIR);
FLAG (S_IFBLK);
FLAG (S_IFREG);
FLAG (S_IFLNK);

FLAG (EPERM);
FLAG (ENOENT);
FLAG (ESRCH);
FLAG (EINTR);
FLAG (EIO);
FLAG (ENXIO);
FLAG (E2BIG);
FLAG (ENOEXEC);
FLAG (EBADF);
FLAG (ECHILD);
FLAG (EAGAIN);
FLAG (ENOMEM);
FLAG (EACCES);
FLAG (EFAULT);
FLAG (EBUSY);
FLAG (EEXIST);
FLAG (EXDEV);
FLAG (ENODEV);
FLAG (ENOTDIR);
FLAG (EISDIR);
FLAG (EINVAL);
FLAG (ENFILE);
FLAG (EMFILE);
FLAG (ENOTTY);
FLAG (ETXTBSY);
FLAG (EFBIG);
FLAG (ENOSPC);
FLAG (ESPIPE);
FLAG (EROFS);
FLAG (EMLINK);
FLAG (EPIPE);
FLAG (EDOM);
FLAG (ERANGE);
FLAG (ENOMSG);
FLAG (EIDRM);
FLAG (EDEADLK);
FLAG (ENOLCK);
FLAG (ENOSTR);
FLAG (ENODATA);
FLAG (ETIME);
FLAG (ENOSR);
FLAG (ENOLINK);
FLAG (EPROTO);
FLAG (EMULTIHOP);
FLAG (EBADMSG);
FLAG (EFTYPE);
FLAG (ENOSYS);
FLAG (ENOTEMPTY);
FLAG (ENAMETOOLONG);
FLAG (ELOOP);
FLAG (EOPNOTSUPP);
FLAG (EPFNOSUPPORT);
FLAG (ECONNRESET);
FLAG (ENOBUFS);
FLAG (EAFNOSUPPORT);
FLAG (EPROTOTYPE);
FLAG (ENOTSOCK);
FLAG (ENOPROTOOPT);
FLAG (ECONNREFUSED);
FLAG (EADDRINUSE);
FLAG (ECONNABORTED);
FLAG (ENETUNREACH);
FLAG (ENETDOWN);
FLAG (ETIMEDOUT);
FLAG (EHOSTDOWN);
FLAG (EHOSTUNREACH);
FLAG (EINPROGRESS);
FLAG (EALREADY);
FLAG (EDESTADDRREQ);
FLAG (EMSGSIZE);
FLAG (EPROTONOSUPPORT);
FLAG (EADDRNOTAVAIL);
FLAG (ENETRESET);
FLAG (EISCONN);
FLAG (ENOTCONN);
FLAG (ETOOMANYREFS);
FLAG (EDQUOT);
FLAG (ESTALE);
FLAG (ENOTSUP);
FLAG (EILSEQ);
FLAG (EOVERFLOW);
FLAG (ECANCELED);
FLAG (ENOTRECOVERABLE);
FLAG (EOWNERDEAD);
FLAG (EWOULDBLOCK);

}
}
