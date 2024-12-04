void sendModbusRequest(int address, int function, int start, int count) {
  byte request[] = {address, function, start >> 8, start & 0xFF, count >> 8, count & 0xFF};
  byte crc = calculateCRC(request, sizeof(request) - 2);
  rs485.write(request, sizeof(request));
  rs485.write(crc);
}


byte calculateCRC(byte* data, int length) {
  unsigned int crc = 0xFFFF;
  for (int i = 0; i < length; ++i) {
    crc ^= data[i];
    for (int j = 0; j < 8; ++j) {
      if (crc & 1) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}
