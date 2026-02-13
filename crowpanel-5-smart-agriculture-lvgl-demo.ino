#define LV_CONF_INCLUDE_SIMPLE
#include <lvgl.h>
#include "gfx_conf.h"

// ===== Display Buffers / Driver =====
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[800 * 20];
static lv_color_t buf2[800 * 20];
static lv_disp_drv_t disp_drv;

// ===== UI Objects =====
static lv_obj_t *arc_temp, *arc_hum, *arc_soil, *arc_o2;
static lv_obj_t *lbl_temp, *lbl_hum, *lbl_soil, *lbl_o2;
static lv_obj_t *lbl_status;

// ===== Demo values =====
static float v_temp = 27.5f;
static float v_hum  = 68.0f;
static float v_soil = 42.0f;
static float v_o2   = 20.6f;

// ================= Display Flush =================
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;

  tft.pushImageDMA(area->x1, area->y1, w, h,
                   (lgfx::rgb565_t *)&color_p->full);

  lv_disp_flush_ready(disp);
}

// ================= Create Gauge Card =================
static lv_obj_t* create_gauge_card(lv_obj_t *parent,
                                  int x, int y,
                                  const char *title,
                                  lv_obj_t **arc_out,
                                  lv_obj_t **value_label_out,
                                  int minv, int maxv,
                                  uint32_t arc_color_hex)
{
  lv_obj_t *card = lv_obj_create(parent);
  lv_obj_set_size(card, 360, 190);
  lv_obj_set_pos(card, x, y);
  lv_obj_set_style_radius(card, 22, 0);
  lv_obj_set_style_bg_color(card, lv_color_hex(0x111827), 0);
  lv_obj_set_style_border_width(card, 1, 0);
  lv_obj_set_style_border_color(card, lv_color_hex(0x1F2937), 0);
  lv_obj_set_style_pad_all(card, 14, 0);

  // Title
  lv_obj_t *t = lv_label_create(card);
  lv_label_set_text(t, title);
  lv_obj_set_style_text_color(t, lv_color_hex(0xE5E7EB), 0);
  lv_obj_set_style_text_font(t, &lv_font_montserrat_18, 0);
  lv_obj_align(t, LV_ALIGN_TOP_LEFT, 8, 2);

  // Arc
  lv_obj_t *arc = lv_arc_create(card);
  lv_obj_set_size(arc, 140, 140);
  lv_obj_align(arc, LV_ALIGN_LEFT_MID, 10, 18);
  lv_arc_set_range(arc, minv, maxv);
  lv_arc_set_bg_angles(arc, 135, 45);
  lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
  lv_obj_set_style_arc_width(arc, 14, LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc, 14, LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(arc, lv_color_hex(0x334155), LV_PART_MAIN);
  lv_obj_set_style_arc_color(arc, lv_color_hex(arc_color_hex), LV_PART_INDICATOR);

  // Value
  lv_obj_t *val = lv_label_create(card);
  lv_label_set_text(val, "--");
  lv_obj_set_style_text_font(val, &lv_font_montserrat_36, 0);
  lv_obj_set_style_text_color(val, lv_color_hex(0xF9FAFB), 0);
  lv_obj_align(val, LV_ALIGN_LEFT_MID, 170, -5);

  // Subtext
  lv_obj_t *sub = lv_label_create(card);
  lv_label_set_text(sub, "Live reading");
  lv_obj_set_style_text_font(sub, &lv_font_montserrat_16, 0);
  lv_obj_set_style_text_color(sub, lv_color_hex(0x9CA3AF), 0);
  lv_obj_align(sub, LV_ALIGN_LEFT_MID, 172, 40);

  *arc_out = arc;
  *value_label_out = val;
  return card;
}

// ================= Build UI =================
void create_ui()
{
  lv_obj_t *scr = lv_scr_act();
  lv_obj_set_style_bg_color(scr, lv_color_hex(0x0B1220), 0);

  lv_obj_t *title = lv_label_create(scr);
  lv_label_set_text(title, "SMART AGRICULTURE DASHBOARD");
  lv_obj_set_style_text_color(title, lv_color_hex(0xE5E7EB), 0);
  lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 26, 18);

  lv_obj_t *subtitle = lv_label_create(scr);
  lv_label_set_text(subtitle, "Field Node • Real-time crop environment");
  lv_obj_set_style_text_color(subtitle, lv_color_hex(0x94A3B8), 0);
  lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_16, 0);
  lv_obj_align(subtitle, LV_ALIGN_TOP_LEFT, 26, 48);

  create_gauge_card(scr, 30, 90,  "Temperature (°C)", &arc_temp, &lbl_temp, 0, 50,  0xF87171);
  create_gauge_card(scr, 410, 90, "Humidity (%)",     &arc_hum,  &lbl_hum,  0, 100, 0x60A5FA);
  create_gauge_card(scr, 30, 290, "Soil Moisture (%)",&arc_soil, &lbl_soil, 0, 100, 0x34D399);
  create_gauge_card(scr, 410, 290,"Oxygen (%)",       &arc_o2,   &lbl_o2,   0, 25,  0xA78BFA);

  lv_obj_t *bar = lv_obj_create(scr);
  lv_obj_set_size(bar, 740, 52);
  lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -14);
  lv_obj_set_style_radius(bar, 16, 0);
  lv_obj_set_style_bg_color(bar, lv_color_hex(0x0F172A), 0);

  lbl_status = lv_label_create(bar);
  lv_obj_set_style_text_font(lbl_status, &lv_font_montserrat_16, 0);
  lv_obj_set_style_text_color(lbl_status, lv_color_hex(0xC7D2FE), 0);
  lv_obj_align(lbl_status, LV_ALIGN_LEFT_MID, 16, 0);

  update_dashboard();   // 🔥 IMPORTANT initial render
}

// ================= Update Dashboard =================
static void update_dashboard()
{
  lv_arc_set_value(arc_temp, (int)v_temp);
  lv_arc_set_value(arc_hum,  (int)v_hum);
  lv_arc_set_value(arc_soil, (int)v_soil);
  lv_arc_set_value(arc_o2,   (int)v_o2);

  static char b[64];

  snprintf(b, sizeof(b), "%.1f", v_temp);
  lv_label_set_text(lbl_temp, b);

  snprintf(b, sizeof(b), "%.0f", v_hum);
  lv_label_set_text(lbl_hum, b);

  snprintf(b, sizeof(b), "%.0f", v_soil);
  lv_label_set_text(lbl_soil, b);

  snprintf(b, sizeof(b), "%.1f", v_o2);
  lv_label_set_text(lbl_o2, b);

  snprintf(b, sizeof(b),
           "Status: OK • Temp %.1f°C • Hum %.0f%% • Soil %.0f%% • O2 %.1f%%",
           v_temp, v_hum, v_soil, v_o2);

  lv_label_set_text(lbl_status, b);
}

// ================= Simulated Sensor =================
static void simulate_values()
{
  v_temp += 0.1f; if (v_temp > 33.0f) v_temp = 26.0f;
  v_hum  += 0.3f; if (v_hum  > 85.0f) v_hum  = 60.0f;
  v_soil -= 0.6f; if (v_soil < 18.0f) v_soil = 55.0f;
  v_o2   += 0.02f;if (v_o2   > 21.0f) v_o2   = 20.4f;
}

void setup()
{
  Serial.begin(115200);

  tft.begin();
  tft.fillScreen(0x0000);

  lv_init();

  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, 800 * 20);

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 800;
  disp_drv.ver_res = 480;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  create_ui();
}

void loop()
{
  lv_timer_handler();

  static uint32_t last = 0;
  uint32_t now = millis();

  if (now - last >= 1000)
  {
    last = now;
    simulate_values();
    update_dashboard();
  }

  delay(5);
}
