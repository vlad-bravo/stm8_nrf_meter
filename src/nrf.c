#include "functions.h"

uint8_t payload[16];

unsigned char nrf_send_read(uint8_t snt)
{
  uint8_t dat;
  
  while (SPI_GetFlagStatus(SPI_FLAG_BSY) == SET) { }
  SPI_SendData(snt);
  while (SPI_GetFlagStatus(SPI_FLAG_BSY) == SET) { }
  dat = SPI_ReceiveData();
  return dat;
}

void deinitNRF(void)
{
  
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regEN_RXADDR);
  nrf_send_read(0<<ERX_P5 | 0<<ERX_P4 | 0<<ERX_P3 | 0<<ERX_P2 | 0<<ERX_P1 | 0<<ERX_P0);
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regEN_AA);
  nrf_send_read(0<<ENAA_P5 | 0<<ENAA_P4 | 0<<ENAA_P3 | 0<<ENAA_P2 | 0<<ENAA_P1 | 0<<ENAA_P0); // disable auto-ack
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regSETUP_RETR);
  nrf_send_read(0x00); // auto retransmit off
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regFEATURE);
  nrf_send_read(0<<EN_DPL | 0<<EN_ACK_PAY | 0<<EN_DYN_ACK); // disable ack payload
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regDYNPD);
  nrf_send_read(0<<DPL_P5 | 0<<DPL_P4 | 0<<DPL_P3 | 0<<DPL_P2 | 0<<DPL_P1 | 0<<DPL_P0); // disable dyn pl
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + cmdACTIVATE);
  nrf_send_read(0x73); // Use the same command and data to deactivate the registers again
  CSN_HIGH();

}

void initNRF_meter_my(void)
{
  CSN_HIGH();
  CE_LOW();

  CSN_LOW();
  nrf_send_read(cmdNOP);
  CSN_HIGH();
  
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regCONFIG);
  nrf_send_read(1<<MASK_RX_DR | 1<<MASK_TX_DS | 1<<MASK_MAX_RT | 1<<EN_CRC | 0<<CRCO | 1<<PWR_UP | 1<<PRIM_RX); // PRX, CRC enabled
  CSN_HIGH();

  //delay(10000);

  //CSN_LOW();
  //nrf_send_read(cmdNOP);   //	1,5 ms after PWR_UP = 1
  //CSN_HIGH();
  
  //CSN_LOW();
  //nrf_send_read(cmdNOP);
  //CSN_HIGH();

  //CSN_LOW();
  //nrf_send_read(cmdWRITE_REG + regSETUP_RETR);
  //nrf_send_read(0x00); // auto retransmit off
  //CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regSETUP_AW);
  nrf_send_read(0x03); // address width = 5 bytes
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regEN_AA);
  nrf_send_read(0<<ENAA_P5 | 0<<ENAA_P4 | 0<<ENAA_P3 | 0<<ENAA_P2 | 1<<ENAA_P1 | 0<<ENAA_P0); // disable auto-ack for all channels
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRF_SETUP);
  nrf_send_read(0<<CONT_WAVE | 0<<RF_DR_LOW | 0<<PLL_LOCK | 1<<RF_DR_HIGH | 0<<RF_PWR2 | 0<<RF_PWR1); // [RF_DR_LOW, RF_DR_HIGH]: ‘00’ – 1Mbps; ‘01’ – 2Mbps; ‘10’ – 250kbps; ‘11’ – Reserved
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRF_CH);
  nrf_send_read(120);
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regEN_RXADDR);
  nrf_send_read(0<<ERX_P5 | 0<<ERX_P4 | 0<<ERX_P3 | 0<<ERX_P2 | 1<<ERX_P1 | 1<<ERX_P0);
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_PW_P0);
  nrf_send_read(32);  // 32 byte payload
  CSN_HIGH();

  // Pipe 0 for meter mode
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_ADDR_P0);
  nrf_send_read(0x5C);  // LSByte is written first
  nrf_send_read(0xE7);
  nrf_send_read(0xE3);
  nrf_send_read(0xE7);
  nrf_send_read(0xC5);
  CSN_HIGH();

  // Pipe 1 for command mode
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_ADDR_P1);
  nrf_send_read(0xC3);  // LSByte is written first
  nrf_send_read(0xE7);
  nrf_send_read(0xE3);
  nrf_send_read(0xE7);
  nrf_send_read(0xC5);
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regSETUP_RETR);
  nrf_send_read(0xFF); // auto retransmit
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + cmdACTIVATE);
  nrf_send_read(0x73); // enable R_RX_PL_WID W_ACK_PAYLOAD W_TX_PAYLOAD_NOACK
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regFEATURE);
  nrf_send_read(1<<EN_DPL | 1<<EN_ACK_PAY | 0<<EN_DYN_ACK); // enable ack payload
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regDYNPD);
  nrf_send_read(0<<DPL_P5 | 0<<DPL_P4 | 0<<DPL_P3 | 0<<DPL_P2 | 1<<DPL_P1 | 0<<DPL_P0); // enable dyn pl
  CSN_HIGH();

  CE_HIGH();     // start receiver
}

void initNRF_meter(void)
{
  CSN_HIGH();
  CE_LOW();

  CSN_LOW();
  nrf_send_read(cmdNOP);
  CSN_HIGH();

  // Clear status flags
  //nrf24_write_reg(STATUS, 0x70);
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regSTATUS);
  nrf_send_read(0x70);
  CSN_HIGH();

  // Configuration
  //nrf24_write_reg(CONFIG, 0x0F); // CRC 2 ?????, ??? RX/TX, ????? ??????????
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regCONFIG);
  nrf_send_read(0x0F);
  CSN_HIGH();
  //nrf24_write_reg(EN_AA, 0x01);   // Auto Ack on pipe0
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regEN_AA);
  nrf_send_read(0x01);
  CSN_HIGH();
  //nrf24_write_reg(EN_RXADDR, 0x01); // Enable pipe0
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regEN_RXADDR);
  nrf_send_read(0x01);
  CSN_HIGH();
  //nrf24_write_reg(SETUP_AW, 0x03);  // 5 ???? ??????
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regSETUP_AW);
  nrf_send_read(0x03);
  CSN_HIGH();
  //nrf24_write_reg(SETUP_RETR, 0x0F); // 15 ???????, 1500 ??? ????????
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regSETUP_RETR);
  nrf_send_read(0x0F);
  CSN_HIGH();
  //nrf24_write_reg(RF_CH, 0x4C);      // ????? 76
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRF_CH);
  nrf_send_read(0x4C);
  CSN_HIGH();
  //nrf24_write_reg(RF_SETUP, 0x0F);   // 250 kbps, max power (???????? ??? ?????? ??????)
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRF_SETUP);
  nrf_send_read(0x0F);
  CSN_HIGH();

  // Addresses
  //nrf24_write_reg_buf(RX_ADDR_P0, TX_ADDR_BYTES, 5);
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_ADDR_P0);
  nrf_send_read(0xC3);  // LSByte is written first
  nrf_send_read(0xE7);
  nrf_send_read(0xE3);
  nrf_send_read(0xE7);
  nrf_send_read(0xC5);
  CSN_HIGH();
  //nrf24_write_reg_buf(TX_ADDR,     TX_ADDR_BYTES, 5);
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regTX_ADDR);
  nrf_send_read(0xC3);  // LSByte is written first
  nrf_send_read(0xE7);
  nrf_send_read(0xE3);
  nrf_send_read(0xE7);
  nrf_send_read(0xC5);
  CSN_HIGH();

  // Payload size (dynamic)
  //nrf24_write_reg(RX_PW_P0, 0); // 0 — dynamic payload
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_PW_P0);
  nrf_send_read(0x00);
  CSN_HIGH();
  //nrf24_write_reg(DYNPD, 0x01); // Dynamic payload on pipe0
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regDYNPD);
  nrf_send_read(0x01);
  CSN_HIGH();
  //nrf24_write_reg(FEATURE, 0x06); // Enable dynamic payload + ACK payload
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regFEATURE);
  nrf_send_read(0x06);
  CSN_HIGH();

  // ????????? ????????? ACK payload (????? ????????? ? loop)
  //const char ack_payload[] = "ACK!";
  //nrf24_cs_low();
  //spi_transfer(W_ACK_PAYLOAD | 0); // pipe 0
  //for (uint8_t i = 0; i < sizeof(ack_payload); i++) {
    //spi_transfer(ack_payload[i]);
  //}
  //nrf24_cs_high();
  CSN_LOW();
  nrf_send_read(cmdW_ACK_PAYLOAD | 0);      // Payload for pipe 0 - command
  for (uint8_t i=0; i<16; i++) nrf_send_read(payload[i]);
  CSN_HIGH();

  // ???????? ?????
  //nrf24_write_reg(CONFIG, 0x0F); // PRIM_RX = 1 (??? 0 = 1 ? RX)
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regCONFIG);
  nrf_send_read(0x0F);
  CSN_HIGH();
  //nrf24_ce_high();
  CE_HIGH();     // start receiver

  //////////////////////////////////////////////////
  if (0) {
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regCONFIG);
  nrf_send_read(1<<MASK_RX_DR | 1<<MASK_TX_DS | 1<<MASK_MAX_RT | 1<<EN_CRC | 0<<CRCO | 1<<PWR_UP | 1<<PRIM_RX); // PRX, CRC enabled
  CSN_HIGH();

  //delay(10000);

  //CSN_LOW();
  //nrf_send_read(cmdNOP);   //	1,5 ms after PWR_UP = 1
  //CSN_HIGH();
  
  //CSN_LOW();
  //nrf_send_read(cmdNOP);
  //CSN_HIGH();

  //CSN_LOW();
  //nrf_send_read(cmdWRITE_REG + regSETUP_RETR);
  //nrf_send_read(0x00); // auto retransmit off
  //CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regSETUP_AW);
  nrf_send_read(0x03); // address width = 5 bytes
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regEN_AA);
  nrf_send_read(0<<ENAA_P5 | 0<<ENAA_P4 | 0<<ENAA_P3 | 0<<ENAA_P2 | 1<<ENAA_P1 | 0<<ENAA_P0); // disable auto-ack for all channels
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRF_SETUP);
  nrf_send_read(0<<CONT_WAVE | 0<<RF_DR_LOW | 0<<PLL_LOCK | 1<<RF_DR_HIGH | 0<<RF_PWR2 | 0<<RF_PWR1); // [RF_DR_LOW, RF_DR_HIGH]: ‘00’ – 1Mbps; ‘01’ – 2Mbps; ‘10’ – 250kbps; ‘11’ – Reserved
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRF_CH);
  nrf_send_read(120);
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regEN_RXADDR);
  nrf_send_read(0<<ERX_P5 | 0<<ERX_P4 | 0<<ERX_P3 | 0<<ERX_P2 | 1<<ERX_P1 | 1<<ERX_P0);
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_PW_P0);
  nrf_send_read(32);  // 32 byte payload
  CSN_HIGH();

  // Pipe 0 for meter mode
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_ADDR_P0);
  nrf_send_read(0x5C);  // LSByte is written first
  nrf_send_read(0xE7);
  nrf_send_read(0xE3);
  nrf_send_read(0xE7);
  nrf_send_read(0xC5);
  CSN_HIGH();

  // Pipe 1 for command mode
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_ADDR_P1);
  nrf_send_read(0xC3);  // LSByte is written first
  nrf_send_read(0xE7);
  nrf_send_read(0xE3);
  nrf_send_read(0xE7);
  nrf_send_read(0xC5);
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regSETUP_RETR);
  nrf_send_read(0xFF); // auto retransmit
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + cmdACTIVATE);
  nrf_send_read(0x73); // enable R_RX_PL_WID W_ACK_PAYLOAD W_TX_PAYLOAD_NOACK
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regFEATURE);
  nrf_send_read(1<<EN_DPL | 1<<EN_ACK_PAY | 0<<EN_DYN_ACK); // enable ack payload
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regDYNPD);
  nrf_send_read(0<<DPL_P5 | 0<<DPL_P4 | 0<<DPL_P3 | 0<<DPL_P2 | 1<<DPL_P1 | 0<<DPL_P0); // enable dyn pl
  CSN_HIGH();

  CE_HIGH();     // start receiver
  }
}

void initNRF_sniffer(void)
{
  CSN_HIGH();
  CE_LOW();

  CSN_LOW();
  nrf_send_read(cmdNOP);
  CSN_HIGH();
  
  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regCONFIG);
  nrf_send_read(0<<MASK_RX_DR | 0<<MASK_TX_DS | 0<<MASK_MAX_RT | 1<<EN_CRC | 0<<CRCO | 1<<PWR_UP | 1<<PRIM_RX); // PRX, CRC enabled
  CSN_HIGH();

//  osDelay(2);   //	1,5 ms after PWR_UP = 1

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regSETUP_AW);
  nrf_send_read(0x03); // address width = 5 bytes
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regEN_AA);
  nrf_send_read(1<<ENAA_P5 | 0<<ENAA_P4 | 0<<ENAA_P3 | 0<<ENAA_P2 | 0<<ENAA_P1 | 0<<ENAA_P0); // disable auto-ack for all channels
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRF_SETUP);
  nrf_send_read(0<<CONT_WAVE | 1<<RF_DR_LOW | 0<<PLL_LOCK | 0<<RF_DR_HIGH | 1<<RF_PWR2 | 1<<RF_PWR1); // [RF_DR_LOW, RF_DR_HIGH]: ‘00’ – 1Mbps; ‘01’ – 2Mbps; ‘10’ – 250kbps; ‘11’ – Reserved
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRF_CH);
  nrf_send_read(110);
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regEN_RXADDR);
  nrf_send_read(1<<ERX_P5 | 1<<ERX_P4 | 1<<ERX_P3 | 1<<ERX_P2 | 1<<ERX_P1 | 1<<ERX_P0);
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_PW_P0);
  nrf_send_read(0x04);  // 4 byte payload
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_ADDR_P0);
  nrf_send_read(0xE7);
  nrf_send_read(0xC2);
  nrf_send_read(0xC2);
  nrf_send_read(0xC2);
  nrf_send_read(0xE7);
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_PW_P1);
  nrf_send_read(0x04);  // 4 byte payload
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_ADDR_P1);
  nrf_send_read(0xC2);
  nrf_send_read(0xE7);
  nrf_send_read(0xE7);
  nrf_send_read(0xE7);
  nrf_send_read(0xC2);
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_PW_P2);
  nrf_send_read(0x04);  // 4 byte payload
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_ADDR_P2);
  nrf_send_read(0x2C);  // Only LSB. MSBytes are equal to RX_ADDR_P1
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_PW_P3);
  nrf_send_read(0x04);  // 4 byte payload
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_ADDR_P3);
  nrf_send_read(0x7C);  // Only LSB. MSBytes are equal to RX_ADDR_P1
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_PW_P4);
  nrf_send_read(0x04);  // 4 byte payload
  CSN_HIGH();

  CSN_LOW();
  nrf_send_read(cmdWRITE_REG + regRX_ADDR_P4);
  nrf_send_read(0x5C);  // Only LSB. MSBytes are equal to RX_ADDR_P1
  CSN_HIGH();

//  CSN_LOW();
//  nrf_send_read(cmdWRITE_REG + regRX_PW_P5);
//  nrf_send_read(0x04);  // 4 byte payload
//  CSN_HIGH();




//  CSN_LOW();
//  nrf_send_read(cmdWRITE_REG + regRX_ADDR_P5);
//  nrf_send_read(0x6C);  // Only LSB. MSBytes are equal to RX_ADDR_P1
//  CSN_HIGH();

//  CSN_LOW();
//  nrf_send_read(cmdWRITE_REG + regSETUP_RETR);
//  nrf_send_read(0xFF); // auto retransmit
//  CSN_HIGH();

//  CSN_LOW();
//  nrf_send_read(cmdWRITE_REG + cmdACTIVATE);
//  nrf_send_read(0x73); // enable R_RX_PL_WID W_ACK_PAYLOAD W_TX_PAYLOAD_NOACK
//  CSN_HIGH();

//  CSN_LOW();
//  nrf_send_read(cmdWRITE_REG + regFEATURE);
//  nrf_send_read(1<<EN_DPL | 1<<EN_ACK_PAY | 0<<EN_DYN_ACK); // enable ack payload
//  CSN_HIGH();

//  CSN_LOW();
//  nrf_send_read(cmdWRITE_REG + regDYNPD);
//  nrf_send_read(1<<DPL_P5 | 0<<DPL_P4 | 0<<DPL_P3 | 0<<DPL_P2 | 0<<DPL_P1 | 0<<DPL_P0); // enable dyn pl
//  CSN_HIGH();

  CE_HIGH();     // start receiver
}

void Init_Payload(void)
{
  
      CSN_LOW();
      nrf_send_read(cmdFLUSH_TX);
      CSN_HIGH();
      CSN_LOW();
      nrf_send_read(cmdFLUSH_RX);
      CSN_HIGH();

      CSN_LOW();
      nrf_send_read(cmdW_ACK_PAYLOAD | 1);      // Payload for pipe 1 - command
      for (uint8_t i=0; i<16; i++) nrf_send_read(payload[i]);
      CSN_HIGH();

}
