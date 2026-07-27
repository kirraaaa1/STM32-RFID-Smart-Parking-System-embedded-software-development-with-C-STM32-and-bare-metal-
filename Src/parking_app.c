#include "parking_app.h"
#include "lcd_i2c.h"
#include "servo.h"
#include "systick.h"

static ParkingCard_t parking_cards[PARKING_MAX_CARDS];
static uint8_t registered_card_count = 0U;
static uint8_t occupied_slots = 0U;

static int32_t ParkingApp_FindCard(const RC522_UID_t *uid)
{
    for (uint8_t i = 0U; i < registered_card_count; i++)
    {
        if (RC522_UIDCompare(uid, &parking_cards[i].uid))
        {
            return (int32_t)i;
        }
    }

    return -1;
}

bool ParkingApp_RegisterCard(
    const RC522_UID_t *uid,
    const char *vehicle_name)
{
    if ((uid == 0) ||
        (uid->length == 0U) ||
        (uid->length > RC522_UID_MAX_LENGTH))
    {
        return false;
    }

    if (registered_card_count >= PARKING_MAX_CARDS)
    {
        return false;
    }

    ParkingCard_t *card =
        &parking_cards[registered_card_count];

    card->uid.length = uid->length;

    for (uint8_t i = 0U; i < uid->length; i++)
    {
        card->uid.data[i] = uid->data[i];
    }

    card->vehicle_name = vehicle_name;
    card->is_inside = false;

    registered_card_count++;

    return true;
}

uint8_t ParkingApp_GetOccupiedSlots(void)
{
    return occupied_slots;
}

uint8_t ParkingApp_GetFreeSlots(void)
{
    return PARKING_TOTAL_SLOTS - occupied_slots;
}

bool ParkingApp_IsFull(void)
{
    return occupied_slots >= PARKING_TOTAL_SLOTS;
}

void ParkingApp_UpdateDisplay(void)
{
    LCD_Clear();

    LCD_SetCursor(0U, 0U);
    LCD_WriteString("BAI GIU XE");

    LCD_SetCursor(1U, 0U);
    LCD_WriteString("CON TRONG: ");
    LCD_WriteNumber(ParkingApp_GetFreeSlots());
}

void ParkingApp_Init(void)
{
    occupied_slots = 0U;
    registered_card_count = 0U;

    Servo_CloseGate();
    ParkingApp_UpdateDisplay();
}

ParkingResult_t ParkingApp_ProcessCard(
    const RC522_UID_t *uid)
{
    int32_t card_index = ParkingApp_FindCard(uid);

    if (card_index < 0)
    {
        return PARKING_RESULT_CARD_INVALID;
    }

    ParkingCard_t *card = &parking_cards[card_index];

    if (!card->is_inside)
    {
        /* Xe muốn vào */
        if (ParkingApp_IsFull())
        {
            return PARKING_RESULT_FULL;
        }

        card->is_inside = true;
        occupied_slots++;

        return PARKING_RESULT_ENTRY_GRANTED;
    }

    /* Xe muốn ra */
    card->is_inside = false;

    if (occupied_slots > 0U)
    {
        occupied_slots--;
    }

    return PARKING_RESULT_EXIT_GRANTED;
}

void ParkingApp_Run(void)
{
    RC522_UID_t detected_uid;

    if (!RC522_IsCardPresent())
    {
        return;
    }

    if (!RC522_ReadUID(&detected_uid))
    {
        LCD_Clear();
        LCD_WriteString("LOI DOC THE");

        Delay_ms(1000U);
        ParkingApp_UpdateDisplay();
        return;
    }

    ParkingResult_t result =
        ParkingApp_ProcessCard(&detected_uid);

    LCD_Clear();

    switch (result)
    {
        case PARKING_RESULT_ENTRY_GRANTED:
            LCD_WriteString("MOI XE VAO");

            Servo_OpenGate();
            Delay_ms(3000U);
            Servo_CloseGate();
            break;

        case PARKING_RESULT_EXIT_GRANTED:
            LCD_WriteString("MOI XE RA");

            Servo_OpenGate();
            Delay_ms(3000U);
            Servo_CloseGate();
            break;

        case PARKING_RESULT_FULL:
            LCD_WriteString("BAI DA DAY");
            Delay_ms(1500U);
            break;

        case PARKING_RESULT_CARD_INVALID:
            LCD_WriteString("THE KHONG HOPLE");
            Delay_ms(1500U);
            break;

        default:
            break;
    }

    RC522_Halt();

    ParkingApp_UpdateDisplay();

    /* Chống quẹt một thẻ liên tục */
    Delay_ms(1000U);
}