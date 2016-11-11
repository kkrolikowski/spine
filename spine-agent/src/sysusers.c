#include <stdio.h>
#include <stdlib.h>
#include "commondata.h"
#include "sysusers.h"

void sysUsersInit(sysuser * su) {
    su->active = 0;
    su->expiration = 0;
    su->gecos = NULL;
    su->login = NULL;
    su ->next = NULL;
    su->sha512 = NULL;
    su->shellaccess = 0;
    su->sshkey->key = NULL;
    su->sshkey->next = NULL;
    su->uidgid = 0;
}