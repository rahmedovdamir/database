#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "core.h"
#include "storage.h"

bool save_database(Database* db, const char* filename);

Database* load_database(const char* filename);

#endif