//
// Created by royivri on 12/12/25.
//

#ifndef XPACKET_RECOVERY_TASK_H
#define XPACKET_RECOVERY_TASK_H
#define PACKET_RECOVERY_SLEEP_TIME 5000
#define PACKET_RECOVERY_MAX_RETRIES 5
#define PACKET_RECOVERY_RETRY_DELAY_MS 2000





void xPacketRecoveryTask(void *pvParameters);
#endif //XPACKET_RECOVERY_TASK_H
