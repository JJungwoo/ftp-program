
#include <stdio.h>

/*

Debug Log Level Info

jtrace() : 상세한 디버그 로그 확인 (디버그 모드)
jdebug() : 일반 디버그 로그 확인 (실행 모드)
jerror() : 오류 로그 확인

*/

#define DEBUG 1
#define ERROR 2


static void jtime(char *time_str, size_t time_str_size);

