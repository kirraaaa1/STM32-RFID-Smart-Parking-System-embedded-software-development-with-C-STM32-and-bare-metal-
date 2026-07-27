#ifndef RC522_H
#define RC522_H

#include <stdint.h>
#include <stdbool.h>

#define RC522_UID_MAX_LENGTH 10U

typedef struct
{
    uint8_t data[RC522_UID_MAX_LENGTH];
    uint8_t length;
} RC522_UID_t;

void RC522_Init(void);

bool RC522_IsCardPresent(void);
bool RC522_ReadUID(RC522_UID_t *uid);

bool RC522_UIDCompare(
    const RC522_UID_t *uid1,
    const RC522_UID_t *uid2
);

void RC522_Halt(void);

void RC522_SoftReset(void);
void RC522_AntennaOn(void);
void RC522_AntennaOff(void);

uint8_t RC522_GetVersion(void);

#endif /* RC522_H */