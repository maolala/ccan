#include "tdb2-source.h"
#include <ccan/tap/tap.h>
#include <stdlib.h>
#include <err.h>
#include "tdb1-logging.h"

int main(int argc, char *argv[])
{
	struct tdb1_context *tdb;
	TDB1_DATA key, data;

	plan_tests(10);
	tdb = tdb1_open_ex("run.tdb", 1024, TDB1_CLEAR_IF_FIRST,
			  O_CREAT|O_TRUNC|O_RDWR, 0600, &taplogctx, NULL);

	ok1(tdb);
	key.dsize = strlen("hi");
	key.dptr = (void *)"hi";
	data.dsize = strlen("world");
	data.dptr = (void *)"world";

	ok1(tdb1_store(tdb, key, data, TDB1_MODIFY) < 0);
	ok1(tdb1_error(tdb) == TDB1_ERR_NOEXIST);
	ok1(tdb1_store(tdb, key, data, TDB1_INSERT) == 0);
	ok1(tdb1_store(tdb, key, data, TDB1_INSERT) < 0);
	ok1(tdb1_error(tdb) == TDB1_ERR_EXISTS);
	ok1(tdb1_store(tdb, key, data, TDB1_MODIFY) == 0);

	data = tdb1_fetch(tdb, key);
	ok1(data.dsize == strlen("world"));
	ok1(memcmp(data.dptr, "world", strlen("world")) == 0);
	free(data.dptr);

	key.dsize++;
	data = tdb1_fetch(tdb, key);
	ok1(data.dptr == NULL);
	tdb1_close(tdb);

	return exit_status();
}