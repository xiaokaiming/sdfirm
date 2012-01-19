#ifndef __USB_SCD_H_INCLUDE__
#define __USB_SCD_H_INCLUDE__

#include <target/config.h>
#include <target/generic.h>
#include <target/usb.h>
#include <target/scs.h>

#ifdef CONFIG_SCD_DEBUG
#define scd_debug(tag, val)	dbg_print((tag), (val))
#else
#define scd_debug(tag, val)
#endif

#define SCD_DT_SCD		(USB_TYPE_CLASS | 0x01)

/* Smart Card Device Class */
typedef struct scd_desc {
	uint8_t	 bLength;
#define SCD_DT_SCD_SIZE	0x36

	uint8_t	 bDescriptorType;
	uint16_t bcdCCID;
#define CCID_VERSION_DEFAULT	0x100
#define ICCD_VERSION_DEFAULT	0x110

	uint8_t	 bMaxSlotIndex;
	uint8_t	 bVoltageSupport;
#define SCD_VOLTAGE_5V		0x01
#define SCD_VOLTAGE_3V		0x02
#define SCD_VOLTAGE_1_8V	0x04
#define SCD_VOLTAGE_ALL		(SCD_VOLTAGE_5V | \
				 SCD_VOLTAGE_3V | \
				 SCD_VOLTAGE_1_8V)
#define SCD_VOLTAGE_AUTO	0x00

	uint32_t dwProtocols;
#define SCD_PROTOCOL_NONE	0x0000
#define SCD_PROTOCOL_T0		0x0000
#define SCD_PROTOCOL_T1		0x0001
#define SCD_PROTOCOL_T15	0x000F

	uint32_t dwDefaultClock;
	uint32_t dwMaximumClock;
	uint8_t	 bNumClockSupported;
#define ICCD_FIXED_CLOCK	0x0DFC

	uint32_t dwDataRate;
	uint32_t dwMaxDataRate;
	uint8_t	 bNumDataRatesSupported;
#define ICCD_FIXED_DATARATE		0x2580

	uint32_t dwMaxIFSD;
#define ICCD_FIXED_MAX_IFSD		0xFE

	uint32_t dwSynchProtocols;
#define SCD_SYNCH_PROTO_NONE		0x0000
#define SCD_SYNCH_PROTO_2W		0x0001
#define SCD_SYNCH_PROTO_3W		0x0002
#define SCD_SYNCH_PROTO_I2C		0x0004

	uint32_t dwMechanical;
#define SCD_MECHA_NONE			0x00
#define SCD_MECHA_ACCEPT		0x01
#define SCD_MECHA_EJECTION		0x02
#define SCD_MECHA_CAPTURE		0x04
#define SCD_MECHA_LOCK			0x08
#define SCD_MECHA_NONE			0x00

	uint32_t dwFeatures;
#define SCD_FEATURE_NONE		0x00
#define SCD_FEATURE_AUTO_CONFIG		0x00000002
#define SCD_FEATURE_AUTO_ACTIVATE	0x00000004
#define SCD_FEATURE_AUTO_CLASS		0x00000008
#define SCD_FEATURE_AUTO_FREQ		0x00000010
#define SCD_FEATURE_AUTO_DATA		0x00000020

#define SCD_FEATURE_SLOT_MASK		0x0000000C

#define SCD_FEATURE_AUTO_PPS_PROP	0x00000040
#define SCD_FEATURE_AUTO_PPS_BASE	0x00000080

#define SCD_FEATURE_CAP_CLOCK_STOP	0x00000100
#define SCD_FEATURE_CAP_ACCEPT_NAD	0x00000200
#define SCD_FEATURE_AUTO_IFSD		0x00000400

#define SCD_FEATURE_XCHG_CHAR		0x00000000
#define SCD_FEATURE_XCHG_TPDU		0x00010000
#define SCD_FEATURE_XCHG_APDU		0x00020000
#define SCD_FEATURE_XCHG_APDU_EXT	0x00040000
#define SCD_FEATURE_XCHG_MASK		0x00070000

#define SCD_FEATURE_WAKEUP_ICC		0x00100000

#define ICCD_FEATURE_SPECIFIC		0x00000800
#define ICCD_FEATURE_DEFAULT		(ICCD_FEATURE_SPECIFIC | SCD_FEATURE_AUTO_PPS_PROP)

	uint32_t dwMaxCCIDMessageLength;

	uint8_t	 bClassGetResponse;
	uint8_t	 bClassEnvelope;
#define SCD_MUTE_APDU_CLASS		0x00
#define SCD_ECHO_APDU_CLASS		0xFF

	uint16_t wLcdLayout;
#define SCD_LCD_LAYOUT_NONE		0x0000

	uint8_t	 bPINSupport;
#define SCD_SPE_SUPPORT_NONE		0x00
#define SCD_SPE_SUPPORT_VERIFY		0x01
#define SCD_SPE_SUPPORT_MODIFY		0x02
#define SCD_SPE_SUPPORT_ALL		(SCD_SPE_SUPPORT_VERIFY | \
					 SCD_SPE_SUPPORT_MODIFY)

	uint8_t	 bMaxCCIDBusySlots;
#define ICCD_MAX_BUSY_SLOT		0x01
} scd_desc_t;

struct scd_cmd {
	uint8_t  bMessageType;
	scs_size_t dwLength;
	uint8_t  bSlot;
	uint8_t  bSeq;
	uint8_t  abRFU[3];
};

struct scd_resp {
	uint8_t  bMessageType;
	scs_size_t dwLength;
	/* Slot Status Register */
	uint8_t bStatus;
	/* Slot Error Register */
	uint8_t bError;
	uint8_t abRFU3;
};

#define SCD_SLOT_STATUS_ACTIVE		0x00
#define SCD_SLOT_STATUS_INACTIVE	0x01
#define SCD_SLOT_STATUS_NOTPRESENT	0x02

#ifdef CONFIG_SCD_BULK
#define SCD_HEADER_SIZE			10
#else
#define SCD_HEADER_SIZE			0
#endif

/* SCD_PC2RDR_XFRBLOCK parameters */
struct scd_xb_param {
	scs_err_t dwIccOutErr;
	urb_size_t dwIccOutCnt;
	urb_size_t dwIccExpCnt;
	uint8_t bIccWaitInt;
};

/* SCD_RDR2PC_DATABLOCK parameters */
struct scd_db_param {
	urb_size_t dwIccOutIter;
	scs_err_t dwIccOutErr;
};

struct scd_t0_param {
	uint8_t bmFindexDindex;
	uint8_t bmTCCKST0;
	uint8_t bGuardTimeT0;
	uint8_t bWaitingIntegerT0;
	uint8_t bClockStop;
};

struct scd_t1_param {
	uint8_t bmFindexDindex;
	uint8_t bmTCCKST1;
	uint8_t bGuardTimeT1;
	uint8_t bWaitingIntegerT1;
	uint8_t bClockStop;
	uint8_t bIFSC;
	uint8_t bNadValue;
};

#define SCD_SLOT_STATE_PC2RDR		0x00
#define SCD_SLOT_STATE_ISO7816		0x01
#define SCD_SLOT_STATE_RDR2PC		0x02
#define SCD_SLOT_STATE_SANITY		0x03

#define SCD_CMD_STATUS_SUCC		(0x00 << 6)
#define SCD_CMD_STATUS_FAIL		(0x01 << 6)
#define SCD_CMD_STATUS_TIME_EXT		(0x02 << 6)
#define SCD_CMD_STATUS_MASK		(0xc0)

/* functions should be implemented by SCD generic */
#define scd_slot_success(err)		((err) == SCS_ERR_SUCCESS)
#define scd_slot_progress(err)		((err) == SCS_ERR_PROGRESS)

/* functions should be implemented by SCD protocol */
uint8_t scd_slot_status(void);

void scd_Escape_init(void);
void scd_Escape_out(void);
void scd_Escape_cmp(void);
void scd_Escape_in(void);

#ifdef CONFIG_USB_CCID
#include <target/usb_ccid.h>
#define NR_SCD_QUEUES		NR_CCID_QUEUES
#define scd_CmdOffset_cmp(off)	ccid_CmdOffset_cmp(off)
#define scd_SlotStatus_in()	ccid_SlotStatus_in()
#endif
#ifdef CONFIG_USB_ICCD
#include <target/usb_iccd.h>
#define NR_SCD_QUEUES		NR_SCD_SLOTS
#define scd_CmdOffset_cmp(off)	iccd_CmdOffset_cmp(off)
#define scd_SlotStatus_in()	iccd_SlotStatus_in()
#endif

#endif /* __USB_SCD_H_INCLUDE__ */
