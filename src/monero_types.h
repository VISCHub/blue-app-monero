/* Copyright 2017 Cedric Mesnil <cslashm@gmail.com>, Ledger SAS
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MONERO_TYPES_H
#define MONERO_TYPES_H

#include "os_io_seproxyhal.h"
/* cannot send more that F0 bytes in CCID, why? do not know for now
 *  So set up length to F0 minus 2 bytes for SW
 */
#define MONERO_APDU_LENGTH                       0xFE


/* big private DO */
#define MONERO_EXT_PRIVATE_DO_LENGTH             512
/* will be fixed..1024 is not enougth */
#define MONERO_EXT_CARD_HOLDER_CERT_LENTH        2560
/* random choice */
#define MONERO_EXT_CHALLENGE_LENTH               254
/* accpet long PW, but less than one sha256 block */
#define MONERO_MAX_PW_LENGTH                     12

#define MONERO_KEYS_SLOTS                        3

#define  MONERO_KEY_ATTRIBUTES_LENGTH            12

#define MONERO_RSA_DEFAULT_PUB 0x010001U


#define  CRYPTONOTE_MAINNET_PUBLIC_ADDRESS_BASE58_PREFIX  18
#define  CRYPTONOTE_TESTNET_PUBLIC_ADDRESS_BASE58_PREFIX  53

struct monero_nv_state_s {
  /* magic */
  unsigned char magic[8];

  /* network */
  unsigned char network_id;

  /* view key */
  unsigned char A[32];
  unsigned char a[32];
  
  /* spend key */
  unsigned char B[32];
  unsigned char b[32];

  /* public address */
  char public_address[96];
} ;

typedef struct monero_nv_state_s monero_nv_state_t;

#define MONERO_IO_BUFFER_LENGTH (300)

struct monero_v_state_s {
  /* ------------------------------------------ */
  /* ---                  IO                --- */
  /* ------------------------------------------ */
  
  /* io state*/
  unsigned char   io_cla;
  unsigned char   io_ins;
  unsigned char   io_p1;
  unsigned char   io_p2;
  unsigned char   io_lc;
  unsigned char   io_le;
  unsigned short  io_length;
  unsigned short  io_offset;
  unsigned short  io_mark;
  unsigned char   io_buffer[MONERO_IO_BUFFER_LENGTH];

  
  unsigned int    options;

  /* ------------------------------------------ */
  /* ---            State Machine           --- */
  /* ------------------------------------------ */
  
  /* app state: INS|P1 */
  unsigned int   state;


  #define SIG_REAL 0
  #define SIG_FAKE 1
  unsigned int   sig_mode;

  /* ------------------------------------------ */
  /* ---               Crypo                --- */
  /* ------------------------------------------ */

  /* SPK */
  cx_aes_key_t spk;

  /* Tx key */
  unsigned char R[32];
  unsigned char r[32];

  /* mlsag hash */
  cx_sha3_t     keccakF;  
  cx_sha3_t     keccakH;
  unsigned char H[32];
  unsigned char c[32];

  /* -- track tx-in/out and commitment -- */
  cx_sha256_t   sha256L;
  unsigned char L[32];
  
  cx_sha256_t   sha256C;
  unsigned char C[32];

  /* -- multiple commands memories -- */
  unsigned char Dinout[32];
 

  /* ------------------------------------------ */
  /* ---               UI/UX                --- */
  /* ------------------------------------------ */
  /* menu 0: 95-chars + "<monero: >"  + null */
  char            ux_menu[112];
  // address to display: 95-chars + null
  char            ux_address[96];
  // xmr to display: max pow(2,64) unit, aka 20-chars + '0' + dot + null
  char            ux_amount[23];
  ux_menu_entry_t ui_dogsays[2];

  /* ------------------------------------------ */
  /* ---                DEBUG               --- */
  /* ------------------------------------------ */
  unsigned char rnd;

} ;
typedef struct  monero_v_state_s monero_v_state_t;

#define SIZEOF_TX_VSTATE   (sizeof(monero_v_state_t) - OFFSETOF(monero_v_state_t, state))

/* ---  ...  --- */
#define STATE_ACTIVATE                      0x07
#define IO_OFFSET_END                       (unsigned int)-1
#define IO_OFFSET_MARK                      (unsigned int)-2


#define OPTION_KEEP_r                       0x01

#define ENCRYPTED_PAYMENT_ID_TAIL            0x8d
/* ---  Errors  --- */

#define ERROR(x)                            ((x)<<16)

#define ERROR_IO_OFFSET                     ERROR(1)
#define ERROR_IO_FULL                       ERROR(2)




/* ---  INS  --- */
#define INS_NONE                            0x00
#define INS_GET_KEY                         0x30
#define INS_PUT_KEY                         0x32

#define INS_RESET                           0x02

#define INS_OPEN_TX                         0x50
#define INS_STEALTH                         0x52
#define INS_PROCESS_INPUT                   0x54 
#define INS_PROCESS_OUTPUT                  0x56
#define INS_BLIND                           0x58
#define INS_VALIDATE                        0x5A
#define INS_MLSAG                           0x5C
            
#define INS_GEN_KEY_DERIVATION_DATA         0x70
#define INS_DERIVE_SEC_KEY                  0x72
#define INS_DERIVE_PUB_KEY                  0x74
#define INS_UNBLIND                         0x76
#define INS_AMOUNT_KEY                      0x78
#define INS_VERIFY_KEY                      0x7A
#define INS_GET_CHACHA_PREKEY               0x7C
#define INS_SET_SIGNATURE_MODE              0x7E


#define INS_GET_RESPONSE                    0xc0


/* ---  IO constants  --- */
#define OFFSET_CLA                          0
#define OFFSET_INS                          1
#define OFFSET_P1                           2
#define OFFSET_P2                           3
#define OFFSET_P3                           4
#define OFFSET_CDATA                        5
#define OFFSET_EXT_CDATA                    7


#define SW_OK                               0x9000
#define SW_ALGORITHM_UNSUPPORTED            0x9484

#define SW_BYTES_REMAINING_00               0x6100

#define SW_WARNING_STATE_UNCHANGED          0x6200
#define SW_STATE_TERMINATED                 0x6285

#define SW_MORE_DATA_AVAILABLE              0x6310

#define SW_WRONG_LENGTH                     0x6700

#define SW_LOGICAL_CHANNEL_NOT_SUPPORTED    0x6881
#define SW_SECURE_MESSAGING_NOT_SUPPORTED   0x6882
#define SW_LAST_COMMAND_EXPECTED            0x6883
#define SW_COMMAND_CHAINING_NOT_SUPPORTED   0x6884

#define SW_SECURITY_STATUS_NOT_SATISFIED    0x6982
#define SW_FILE_INVALID                     0x6983
#define SW_PIN_BLOCKED                      0x6983
#define SW_DATA_INVALID                     0x6984
#define SW_CONDITIONS_NOT_SATISFIED         0x6985
#define SW_COMMAND_NOT_ALLOWED              0x6986
#define SW_APPLET_SELECT_FAILED             0x6999

#define SW_WRONG_DATA                       0x6a80
#define SW_FUNC_NOT_SUPPORTED               0x6a81
#define SW_FILE_NOT_FOUND                   0x6a82
#define SW_RECORD_NOT_FOUND                 0x6a83
#define SW_FILE_FULL                        0x6a84
#define SW_INCORRECT_P1P2                   0x6a86
#define SW_REFERENCED_DATA_NOT_FOUND        0x6a88

#define SW_WRONG_P1P2                       0x6b00
#define SW_CORRECT_LENGTH_00                0x6c00
#define SW_INS_NOT_SUPPORTED                0x6d00
#define SW_CLA_NOT_SUPPORTED                0x6e00

#define SW_UNKNOWN                          0x6f00


#endif
