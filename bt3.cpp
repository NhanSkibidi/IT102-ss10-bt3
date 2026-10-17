#include <stdio.h>
#include <string.h>

#define MAX_ZONES 10
#define MAX_QRS 50
#define OK 1
#define ERROR 0

struct TicketZone {
    int zone_id;
    char zone_name[30];
    float ticket_price;
    int total_capacity;
    int sold_tickets;
};

struct QrCheckIn {
    char qr_code[30];
    int zone_id;
    int is_used;
};

float tinhGiaVe(float giaGoc, int is_early_bird);

int timKhuVuc(
    const struct TicketZone zones[],
    int total_zones,
    int zone_id
);

int timQr(
    const struct QrCheckIn qrs[],
    int total_qrs,
    const char qr_code[]
);

int banVe(
    struct TicketZone zones[],
    int total_zones,
    int zone_id,
    int quantity,
    int is_early_bird,
    float *tongTien
);

int checkInQr(
    struct QrCheckIn qrs[],
    int total_qrs,
    const char qr_code[]
);

int main(void) {
    struct TicketZone zones[MAX_ZONES] = {
        {1, "VIP", 2000000.0f, 10, 8},
        {2, "Zone A", 1000000.0f, 20, 5}
    };

    struct QrCheckIn qrs[MAX_QRS] = {
        {"QR1001", 1, 0},
        {"QR1002", 1, 0},
        {"QR1003", 2, 0}
    };

    struct TicketZone emptyZones[MAX_ZONES];
    struct QrCheckIn emptyQrs[MAX_QRS];

    int total_zones = 2;
    int total_qrs = 3;
    float tongTien = 0.0f;
    int i;

    printf("=== TICKETBOX LIVE CONCERT ===\n");

    printf("\n1. BAN 2 VE ZONE A, CO EARLY BIRD\n");
    if (banVe(zones, total_zones, 2, 2, 1, &tongTien) == OK) {
        printf("Tong tien: %.0f VND\n", tongTien);
    }

    printf("\n2. KIEM TRA SO LUONG AM\n");
    banVe(zones, total_zones, 2, -2, 0, &tongTien);

    printf("\n3. KIEM TRA SO LUONG BANG 0\n");
    banVe(zones, total_zones, 2, 0, 0, &tongTien);

    printf("\n4. KIEM TRA MUA 5 VE\n");
    banVe(zones, total_zones, 2, 5, 0, &tongTien);

    printf("\n5. KIEM TRA VUOT TON KHO VIP\n");
    banVe(zones, total_zones, 1, 3, 0, &tongTien);

    printf("\n6. KIEM TRA ZONE KHONG TON TAI\n");
    banVe(zones, total_zones, 99, 1, 0, &tongTien);

    printf("\n7. CHECK-IN QR1001 LAN DAU\n");
    checkInQr(qrs, total_qrs, "QR1001");

    printf("\n8. QUET LAI QR1001\n");
    checkInQr(qrs, total_qrs, "QR1001");

    printf("\n9. CHECK-IN QR KHONG TON TAI\n");
    checkInQr(qrs, total_qrs, "QR9999");

    printf("\n10. KIEM TRA DANH SACH RONG\n");
    banVe(emptyZones, 0, 1, 1, 0, &tongTien);
    checkInQr(emptyQrs, 0, "QR1001");

    printf("\n11. TON KHO SAU CAC GIAO DICH\n");
    for (i = 0; i < total_zones; i++) {
        printf(
            "Khu vuc: %s | Da ban: %d/%d ve | Con lai: %d ve\n",
            zones[i].zone_name,
            zones[i].sold_tickets,
            zones[i].total_capacity,
            zones[i].total_capacity - zones[i].sold_tickets
        );
    }

    return 0;
}

float tinhGiaVe(float giaGoc, int is_early_bird) {
    if (is_early_bird == 1) {
        return giaGoc * 0.85f;
    }

    return giaGoc;
}

int timKhuVuc(
    const struct TicketZone zones[],
    int total_zones,
    int zone_id
) {
    int i;

    if (total_zones <= 0 || total_zones > MAX_ZONES) {
        return -1;
    }

    for (i = 0; i < total_zones; i++) {
        if (zones[i].zone_id == zone_id) {
            return i;
        }
    }

    return -1;
}

int timQr(
    const struct QrCheckIn qrs[],
    int total_qrs,
    const char qr_code[]
) {
    int i;

    if (total_qrs <= 0 || total_qrs > MAX_QRS) {
        return -1;
    }

    for (i = 0; i < total_qrs; i++) {
        if (strcmp(qrs[i].qr_code, qr_code) == 0) {
            return i;
        }
    }

    return -1;
}

int banVe(
    struct TicketZone zones[],
    int total_zones,
    int zone_id,
    int quantity,
    int is_early_bird,
    float *tongTien
) {
    int viTri;
    float giaThanhToan;

    if (tongTien == NULL) {
        printf("[LOI] Tham so tong tien khong hop le!\n");
        return ERROR;
    }

    *tongTien = 0.0f;

    if (quantity < 1 || quantity > 4) {
        printf("[LOI] So luong mua phai tu 1 den 4 ve!\n");
        return ERROR;
    }

    if (is_early_bird != 0 && is_early_bird != 1) {
        printf("[LOI] Trang thai Early Bird khong hop le!\n");
        return ERROR;
    }

    if (total_zones <= 0) {
        printf("[LOI] Danh sach khu vuc dang rong!\n");
        return ERROR;
    }

    viTri = timKhuVuc(zones, total_zones, zone_id);

    if (viTri == -1) {
        printf("[LOI] Khong tim thay khu vuc %d!\n", zone_id);
        return ERROR;
    }

    if (zones[viTri].total_capacity < 0 ||
        zones[viTri].sold_tickets < 0 ||
        zones[viTri].sold_tickets > zones[viTri].total_capacity) {
        printf("[LOI] Du lieu ton kho khong hop le!\n");
        return ERROR;
    }

    if (zones[viTri].sold_tickets + quantity >
        zones[viTri].total_capacity) {
        printf("[LOI] Khong du ve trong khu vuc %s!\n",
               zones[viTri].zone_name);
        return ERROR;
    }

    giaThanhToan = tinhGiaVe(
        zones[viTri].ticket_price,
        is_early_bird
    );

    *tongTien = giaThanhToan * quantity;
    zones[viTri].sold_tickets += quantity;

    printf("[THANH CONG] Da ban %d ve tai khu vuc %s.\n",
           quantity, zones[viTri].zone_name);

    return OK;
}

int checkInQr(
    struct QrCheckIn qrs[],
    int total_qrs,
    const char qr_code[]
) {
    int viTri;

    if (total_qrs <= 0) {
        printf("[LOI] Danh sach QR dang rong!\n");
        return ERROR;
    }

    viTri = timQr(qrs, total_qrs, qr_code);

    if (viTri == -1) {
        printf("[LOI] Ma QR %s khong ton tai!\n", qr_code);
        return ERROR;
    }

    if (qrs[viTri].is_used == 1) {
        printf("[CANH BAO] QR %s da duoc su dung!\n", qr_code);
        return ERROR;
    }

    if (qrs[viTri].is_used != 0) {
        printf("[LOI] Trang thai QR khong hop le!\n");
        return ERROR;
    }

    qrs[viTri].is_used = 1;

    printf("[THANH CONG] Check-in QR %s thanh cong.\n", qr_code);

    return OK;
}