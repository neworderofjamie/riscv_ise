#pragma once

// Standard C includes
#include <cstdint>

#define PSEE_REGISTER_DEF(NAME, ADDRESS, STRUCT) struct NAME : RegBase<ADDRESS> STRUCT

template<uint32_t A>
struct RegBase
{
    static inline const uint16_t address = A;
};


PSEE_REGISTER_DEF(ChipId, 0x0014, {
	uint32_t chip_id;
});

PSEE_REGISTER_DEF(DigSoftReset, 0x001C, {
	uint32_t digital_csr_srst :1;
	uint32_t digital_pipe_srst :1;
	uint32_t analog_rstn :1;
	uint32_t pdl_override :1;
});

PSEE_REGISTER_DEF(SysClkCtrl, 0x0204, {
	uint32_t sys_clk_en :1;
	uint32_t sys_clk_switch :1;
	uint32_t phy_clk_off_count :6;
	uint32_t phy_clk_on_count :6;
	uint32_t phy_clk_div2 :1;
	uint32_t sys_clk_auto_mode :1;
});

PSEE_REGISTER_DEF(PllCtrl, 0x0214, {
	uint32_t pl_enable: 1;
});

PSEE_REGISTER_DEF(EvtIcnClkCtrl, 0x0210, {
	uint32_t evt_icn_clk_en :1;
	uint32_t evt_icn_clk_switch :1;
	uint32_t evt_icn_clk_div :8;
	uint32_t esp_clk_en :1;
	uint32_t ro_clk_en :1;
});

PSEE_REGISTER_DEF(IoCtrl2, 0x608, {
	uint32_t sync_en	:1;
 	uint32_t:3;
 	uint32_t sync_enzi :1;
});

PSEE_REGISTER_DEF(TdRoiXArray, 0x2000, {
	uint32_t value;
});

PSEE_REGISTER_DEF(TdRoiYArray, 0x3000, {
	uint32_t value;
});

PSEE_REGISTER_DEF(CpiPipelineControl, 0x8000, {
	uint32_t enable :1;
	uint32_t drop_nbackpressure :1;
	uint32_t:1;
	uint32_t output_fifo_bypass :1;
	uint32_t output_data_format :1;
	uint32_t output_if_mode :1;
	uint32_t output_width :1;
	uint32_t packed_fixed_size_enable: 1;
	uint32_t packed_fixed_rate_enable: 1;
	uint32_t frame_fixed_size_enable: 1;
	uint32_t clk_out_en: 1;
	uint32_t clk_control_inversion: 1;
	uint32_t clk_out_gating_enable: 1;
	uint32_t clk_timeout: 8;
	uint32_t packet_pad_empty_enable :1;
	uint32_t hot_disable_enable :1;
});

PSEE_REGISTER_DEF(Bgen, 0x1000, {
	uint32_t bias_ctrl :7;
	uint32_t:9;
	uint32_t buf_stg :2;
	uint32_t:1;
	uint32_t ibtype_sel :1;
	uint32_t:4;
	uint32_t bias_en :1;
	uint32_t pull_sel :1;
	uint32_t:2;
	uint32_t single :1;
});

PSEE_REGISTER_DEF(BgenCtrl, 0x1208, {
	uint32_t burst_transfer_hv_bank_0 :1;
	uint32_t burst_transfer_hv_bank_1 :1;
	uint32_t burst_transfer_lv_bank_0 :1;
	uint32_t burst_transfer_lv_bank_1 :1;
	uint32_t bias_rstn_hv :1;
	uint32_t bias_rstn_lv :1;
});

//----------------------------------------------------------------------------
// Region of Interest registers
//----------------------------------------------------------------------------
namespace ROI
{
PSEE_REGISTER_DEF(Ctrl, 0x0000, {
	uint32_t:1;
	uint32_t roi_td_en :1;
	uint32_t:3;
	uint32_t td_shadow_trigger :1;
	uint32_t px_iphoto_en :1;
	uint32_t px_row_mon_rstn :1;
	uint32_t:2;
	uint32_t px_sw_rstn :1;
	uint32_t px_roi_halt_programming :1;
	uint32_t:20;
});

PSEE_REGISTER_DEF(MasterCtrl, 0x0034, {
	uint32_t master_en :1;
	uint32_t master_run :1;
	uint32_t master_mode :1;
	uint32_t win_nb :5;
	uint32_t:8;
	uint32_t master_busy :1;
	uint32_t master_done :1;
});

PSEE_REGISTER_DEF(MasterChickenBit, 0x0044, {
	uint32_t driver_register_if_en :1;
	uint32_t hold_time :5;
});


PSEE_REGISTER_DEF(WinArray, 0x0400, {
	uint32_t roi_win_start :9;
	uint32_t:7;
	uint32_t roi_win_end :9;
	uint32_t:7;
});
}

//----------------------------------------------------------------------------
// ERC registers
//----------------------------------------------------------------------------
namespace ERC
{
PSEE_REGISTER_DEF(RefPeriodFlavor, 0x6030, {
	uint32_t reference_period : 10;
	uint32_t:6;
	uint32_t avg_drop_rate_delayed: 1;
});

PSEE_REGISTER_DEF(TdTargetEventCount, 0x602C, {
	uint32_t val;
});

PSEE_REGISTER_DEF(AhvtDroppingControl, 0x6014, {
	uint32_t h_dropping_en :1;
	uint32_t v_dropping_en :1;
	uint32_t t_dropping_en :1;
	uint32_t t_dropping_lut_en :1;
	uint32_t drop_all_td_when_drop_geq : 10;
	uint32_t:17;
	uint32_t status: 1;
});

PSEE_REGISTER_DEF(PipelineControl, 0x6000, {
	uint32_t enable :1;
	uint32_t drop_nbackpressure :1;
	uint32_t bypass :1;
});

PSEE_REGISTER_DEF(DelayFifoFlushAndBypass, 0x60A0, {
	uint32_t en :1;
	uint32_t:30;
	uint32_t status :1;
});

PSEE_REGISTER_DEF(MonitoringEventControl, 0x6034, {
	uint32_t first_module_tag_en :1;
	uint32_t avg_drop_rate_en :1;
	uint32_t in_td_cnt_en :1;
	uint32_t df_td_vect_drop_cnt_en :1;
	uint32_t df_non_td_vect_drop_cnt_en :1;
	uint32_t alldr_evt_drop_cnt_en :1;
	uint32_t hdr_evt_drop_cnt_en :1;
	uint32_t vdr_evt_drop_cnt_en :1;
	uint32_t tdr_evt_drop_cnt_en :1;
	uint32_t erc_td_evt_cnt_en :1;
	uint32_t last_module_tag_en :1;
});

PSEE_REGISTER_DEF(ResetTdropCounterOnMtagFirst, 0x60B4, {
	uint32_t en :1;
});
}

//----------------------------------------------------------------------------
// EDF registers
//----------------------------------------------------------------------------
namespace EDF
{
PSEE_REGISTER_DEF(PipelineControl, 0x7000, {
	uint32_t enable :1;
	uint32_t drop_nbackpressure :1;
	uint32_t bypass :1;
});

PSEE_REGISTER_DEF(Control, 0x7044, {
	uint32_t format :2;
	uint32_t:2;
	uint32_t endianness :1;
});

PSEE_REGISTER_DEF(OutputInterfaceControl, 0x704C, {
	uint32_t:4;
	uint32_t start_of_frame_timeout :12;
	uint32_t:16;
});

PSEE_REGISTER_DEF(ExternalOutputAdapter, 0x7100, {
	uint32_t qos_timeout :16;
	uint32_t atomic_qos_mode :1;
});
}

//----------------------------------------------------------------------------
// SRAM registers
//----------------------------------------------------------------------------
namespace SRAM
{
PSEE_REGISTER_DEF(InitN, 0x00B8, {
	uint32_t afk_initn :1;
	uint32_t ehc_stc_initn :1;
	uint32_t erc_dl_initn :1;
	uint32_t erc_ilg_initn :1;
	uint32_t erc_tdrop_initn :1;
	uint32_t mipi_initn :1;
	uint32_t cpi_initn :1;
	uint32_t imem_initn :1;
	uint32_t dmem_initn :1;
	uint32_t rom_initn :1;
	uint32_t:22;
});

PSEE_REGISTER_DEF(Pd1, 0x00C0, {
	uint32_t dmem_pd :1;
	uint32_t imem_pd :1;
	uint32_t rom_pd :1;
	uint32_t erc_dl_pd :1;
	uint32_t erc_ilg_pd :1;
	uint32_t erc_tdrop_pd :1;
	uint32_t mipi_pd :1;
	uint32_t cp_pd :1;
});
}

//----------------------------------------------------------------------------
// Readout registers
//----------------------------------------------------------------------------
namespace Readout
{
PSEE_REGISTER_DEF(TdCtrl, 0x002C, {
	uint32_t ro_td_act_pdy_drive :3;
	uint32_t ro_td_act_pu_drive :4;
	uint32_t ro_td_sendreq_y_stat_en :1;
	uint32_t ro_td_sendreq_y_rstn :1;
	uint32_t ro_td_int_x_rstn :1;
	uint32_t ro_td_int_y_rstn :1;
	uint32_t ro_td_int_x_stat_en :1;
	uint32_t ro_td_int_y_stat_en :1;
	uint32_t ro_td_addr_y_stat_en :1;
	uint32_t ro_td_addr_y_rstn :1;
	uint32_t ro_td_ack_y_rstn :1;
	uint32_t:1;
	uint32_t ro_td_arb_y_rstn :1;
	uint32_t ro_td_ack_y_set :1;
	uint32_t ro_td_int_x_act_pu :3;
	uint32_t ro_td_reqx_ctrllast_bypass :1;
});
PSEE_REGISTER_DEF(ReadoutCtrl, 0x9000, {
	uint32_t ro_test_pixel_mux_en :1;
	uint32_t ro_self_test_en :1;
	uint32_t cpm_record_mode_en :1;
	uint32_t ro_analog_pipe_en :1;
	uint32_t erc_self_test_en :1;
	uint32_t ro_inv_pol_td :1;
	uint32_t ro_flip_x :1;
	uint32_t ro_flip_y :1;
	uint32_t:1;
	uint32_t ro_digital_pipe_en :1;
	uint32_t ro_avoid_bress_td :1;
	uint32_t:1;
	uint32_t drop_en :1;
	uint32_t drop_on_full_en :1;
	uint32_t delay_ro_td_int_x_act_fal : 4;
	uint32_t delay_ro_td_int_x_act_ris : 4;
});

PSEE_REGISTER_DEF(TimeBaseCtrl, 0x9008, {
	uint32_t time_base_enable :1;
	uint32_t time_base_mode :1;
	uint32_t external_mode :1;
	uint32_t external_mode_enable :1;
	uint32_t us_counter_max :7;
	uint32_t th_every_64us_en :1;
	uint32_t:4;
	uint32_t time_base_srst :1;
});
PSEE_REGISTER_DEF(LpCtrl, 0x9028, {
	uint32_t lp_cnt_en :1;
	uint32_t lp_output_disable :1;
	uint32_t lp_keep_th :1;
});
}

//----------------------------------------------------------------------------
// MIPI CSI registers
//----------------------------------------------------------------------------
namespace MipiCsi
{
PSEE_REGISTER_DEF(Ctrl, 0xB000, {
	uint32_t enable :1;
	uint32_t empty :1;
	uint32_t busy :1;
	uint32_t frame_sync_en :1;
	uint32_t line_sync_en :1;
	uint32_t:3;
	uint32_t channel :2;
	uint32_t data_type :6;
	uint32_t pkt_size :14;
});

PSEE_REGISTER_DEF(FrameCtrl, 0xB010, {
	uint32_t pkt_timeout_en :1;
	uint32_t pkt_fix_rate_en :1;
	uint32_t pkt_fix_size_en :1;
	uint32_t frame_fix_rate_en :1;
	uint32_t frame_fix_size_en :1;
	uint32_t fix_rate_empty_pkt :1;
	uint32_t:26;
});

PSEE_REGISTER_DEF(BlFrame, 0xB024, {
	uint32_t val :24;
	uint32_t:6;
	uint32_t ck_lane_hs :1;
	uint32_t enable :1;
});

//----------------------------------------------------------------------------
// MIPI CSI stat registers
//----------------------------------------------------------------------------
namespace Stat
{
PSEE_REGISTER_DEF(Ctrl, 0xB080, {
	uint32_t enable :1;
	uint32_t trigger :1;
	uint32_t clear :1;
});

PSEE_REGISTER_DEF(FrameCnt, 0xB084, {
	uint32_t val;
});

PSEE_REGISTER_DEF(ByteCnt, 0xB088, {
	uint32_t val;
});

PSEE_REGISTER_DEF(PadCnt, 0xB08C, {
	uint32_t val;
});

PSEE_REGISTER_DEF(PktCnt, 0xB090, {
	uint32_t val;
});

PSEE_REGISTER_DEF(IncPktCnt, 0xB094, {
	uint32_t val;
});

PSEE_REGISTER_DEF(FramePeriod, 0xB098, {
	uint32_t val;
});
}
}

//----------------------------------------------------------------------------
// Mbx registers
//----------------------------------------------------------------------------
namespace Mbx
{
PSEE_REGISTER_DEF(CpuSoftReset, 0xF004, {
	uint32_t cpu_soft_reset :1;
	uint32_t:31;
});


PSEE_REGISTER_DEF(Misc, 0xF010, {
	uint32_t misc;
});
}