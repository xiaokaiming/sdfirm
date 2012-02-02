#include <driver/pn53x.h>

uint8_t pn53x_stub_cmd[PN53X_BUF_SIZE];
uint8_t pn53x_stub_resp[PN53X_BUF_SIZE];
scs_size_t pn53x_stub_nc;
scs_size_t pn53x_stub_ne;
boolean pn53x_stub_ready;
boolean pn53x_stub_is_resp;
boolean pn53x_stub_is_cmd;

void pn53x_xchg_pseudo(void)
{
	scs_off_t i;

	for (i = 0; i < pn53x_stub_nc; i++)
		pn53x_stub_resp[i] = pn53x_stub_cmd[i];
	pn53x_stub_ne = pn53x_stub_nc;
	pn53x_stub_is_resp = true;
	pn53x_stub_ready = true;
}

boolean pn53x_hw_poll_ready(void)
{
	return pn53x_stub_ready;
}

void pn53x_hw_read_cmpl(scs_size_t ne)
{
	if (!pn53x_stub_is_resp && pn53x_stub_is_cmd) {
		pn53x_xchg_pseudo();
	}
	pn53x_stub_ready = false;
}

void pn53x_hw_write_cmpl(scs_size_t nc)
{
	pn53x_stub_nc = nc;

	switch (pn53x_type(pn53x_stub_cmd)) {
	case PN53X_ACK:
		pn53x_stub_is_cmd = false;
		pn53x_stub_is_resp = false;
		pn53x_stub_ne = 0;
		break;
	case PN53X_NAK:
		if (pn53x_stub_ne && pn53x_stub_is_resp)
			pn53x_stub_ready = true;
		break;
	default:
		/* validate LCS, DCS, TFI */
		pn53x_stub_is_cmd = true;
		pn53x_stub_is_resp = false;
		pn53x_stub_resp[0] = 0x00;
		pn53x_stub_resp[1] = 0x00;
		pn53x_stub_resp[2] = 0x00;
		pn53x_stub_resp[3] = 0xFF;
		pn53x_stub_resp[4] = 0x00;
		pn53x_stub_resp[5] = 0x00;
		pn53x_stub_ne = 6;
		pn53x_stub_ready = true;
		break;
	}
}

uint8_t pn53x_hw_xchg_read(scs_off_t index)
{
	if (index < pn53x_stub_ne)
		return pn53x_stub_resp[index];
	return 0;
}

void pn53x_hw_xchg_write(scs_off_t index, uint8_t val)
{
	pn53x_stub_cmd[index] = val;
}

void pn53x_hw_ctrl_init(void)
{
	pn53x_stub_nc = 0;
	pn53x_stub_ne = 0;
	pn53x_stub_ready = false;
	pn53x_stub_is_resp = false;
	pn53x_stub_is_cmd = false;
}
