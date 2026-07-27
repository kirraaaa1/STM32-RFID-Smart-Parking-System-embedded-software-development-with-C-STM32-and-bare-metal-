#ifndef PARKING_APP_H
#define PARKING_APP_H

#include <stdint.h>
#include <stdbool.h>
#include "rc522.h"

/*
 * Tổng số vị trí đỗ xe trong mô hình.
 */
#define PARKING_TOTAL_SLOTS       4U

/*
 * Số lượng thẻ tối đa có thể đăng ký.
 */
#define PARKING_MAX_CARDS         5U

/*
 * Trạng thái kết quả khi xử lý thẻ.
 */
typedef enum
{
    PARKING_RESULT_NONE = 0,
    PARKING_RESULT_CARD_INVALID,
    PARKING_RESULT_ENTRY_GRANTED,
    PARKING_RESULT_EXIT_GRANTED,
    PARKING_RESULT_FULL,
    PARKING_RESULT_READ_ERROR
} ParkingResult_t;

/*
 * Thông tin một thẻ đã đăng ký.
 */
typedef struct
{
    RC522_UID_t uid;
    const char *vehicle_name;
    bool is_inside;
} ParkingCard_t;

/*
 * Khởi tạo trạng thái ứng dụng bãi giữ xe.
 *
 * Ban đầu:
 * - Số xe trong bãi bằng 0
 * - Servo đóng cổng
 * - LCD hiển thị số chỗ trống
 */
void ParkingApp_Init(void);

/*
 * Hàm chạy chính của ứng dụng.
 *
 * Gọi liên tục bên trong while(1).
 */
void ParkingApp_Run(void);

/*
 * Xử lý một UID vừa đọc được.
 */
ParkingResult_t ParkingApp_ProcessCard(
    const RC522_UID_t *uid
);

/*
 * Đăng ký một thẻ vào danh sách.
 *
 * return:
 * true  → đăng ký thành công
 * false → danh sách đầy hoặc UID không hợp lệ
 */
bool ParkingApp_RegisterCard(
    const RC522_UID_t *uid,
    const char *vehicle_name
);

/*
 * Trả về số xe hiện đang ở trong bãi.
 */
uint8_t ParkingApp_GetOccupiedSlots(void);

/*
 * Trả về số vị trí còn trống.
 */
uint8_t ParkingApp_GetFreeSlots(void);

/*
 * Kiểm tra bãi xe đã đầy hay chưa.
 */
bool ParkingApp_IsFull(void);

/*
 * Cập nhật thông tin lên LCD.
 */
void ParkingApp_UpdateDisplay(void);

#endif /* PARKING_APP_H */