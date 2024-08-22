#include "MavlinkSettings.h"
#include "Globals.h"

HardwareSerial CUBE_SERIAL(1);

void MavlinkConnection::init() {
	CUBE_SERIAL.begin(BAUD_RATE, SERIAL_PROTOCOL, CUBE_SERIAL_RX, CUBE_SERIAL_TX);
	fmav_init();
	debug_flag = false;
	debug_prev_millis = 0;
}

uint16_t MavlinkConnection::serialAvailable() {
	uint16_t available = CUBE_SERIAL.available();
	return (available > 0) ? available : 0;
}

void MavlinkConnection::readSerial(uint8_t* c) {
	*c = CUBE_SERIAL.read();
}

uint8_t MavlinkConnection::availSerialBuff(uint16_t counter) {
	return (CUBE_SERIAL.availableForWrite() >= counter) ? 1 : 0;
}

void MavlinkConnection::writeToSerial(uint8_t* buf, uint16_t len) {
	for (uint16_t i = 0; i < len; i++) {
		CUBE_SERIAL.write(buf[i]);
	}
}

void MavlinkConnection::handleMessage(fmav_message_t* msg) {
	switch (msg->msgid) {

	case FASTMAVLINK_MSG_ID_RC_CHANNELS: {
		fmav_rc_channels_t data;
		fmav_msg_rc_channels_decode(&data, msg);
		chan9_pwm = data.chan9_raw;
	}break;

	case FASTMAVLINK_MSG_ID_HEARTBEAT: {
		fmav_heartbeat_t data;
		fmav_msg_heartbeat_decode(&data, msg);
		if (data.autopilot == MAV_AUTOPILOT_INVALID && data.type == MAV_TYPE_GCS) {
			uint32_t debug_millis = millis();
			if ((debug_millis - debug_prev_millis) > DEBUG_DELAY) {
				debug_prev_millis = debug_millis;
				debug_flag = true;
			}
		}
	}break;

	default:
		break;
	}
}

uint8_t MavlinkConnection::groundControlDebug() {
	fmav_statustext_t data;
	data.severity = MAV_SEVERITY_INFO;
	memset(&data.text, 0, FASTMAVLINK_MSG_STATUSTEXT_FIELD_TEXT_LEN);
	memcpy(&data.text, "FIREFLY CONNECTED", STATUS_ARRAY_SIZE);
	data.id = 0;
	data.chunk_seq = 0;
	uint16_t count = fmav_msg_statustext_encode_to_frame_buf(
		tx_buf,
		SYS_ID,
		STATUS_COMP_ID,
		&data,
		&status);
	if (availSerialBuff(count)) {
		writeToSerial(tx_buf, count);
		return 1;
	}
	return 0;
}

void MavlinkConnection::decodeMessage() {
	uint16_t available = serialAvailable();
	for (uint16_t i = 0; i < available; i++) {

		uint8_t c;
		readSerial(&c);

		uint8_t res = fmav_parse_to_msg(&msg, &status, c);

		if (res == FASTMAVLINK_PARSE_RESULT_OK) {
			if (fmav_msg_is_for_me(SYS_ID, COMP_ID, &msg)) {
				handleMessage(&msg);
			}
		}
	}
}

uint8_t MavlinkConnection::requestParameters() {
	uint16_t counter = fmav_msg_request_data_stream_pack(
		&msg,
		SYS_ID,
		COMP_ID,
		VEHICLE_TYPE,
		NULL,
		RC_DATA_STREAM,
		REQUEST_RATE,
		START_STOP_VALUE,
		&status
	);

	if (availSerialBuff(counter)) {
		fmav_msg_to_frame_buf(tx_buf, &msg);
		writeToSerial(tx_buf, counter);
		return 1;
	}
	return 0;
}

void MavlinkConnection::run() {
	decodeMessage();
	if (debug_flag) {
		if (groundControlDebug()) {
			debug_flag = false;
		}
	}

	uint32_t curr_millis = millis();
	if ((curr_millis - prev_millis) > INTERVAL) {
		if (requestParameters()) {
			prev_millis += INTERVAL;
		}
	}
}