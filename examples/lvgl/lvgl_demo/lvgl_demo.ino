

#include <lvgl.h>
#include "PanelLan.h"

// BOARD_SC01_PLUS, BOARD_SC02, BOARD_SC05, BOARD_KC01, BOARD_BC02, BOARD_SC07
PanelLan tft(BOARD_SC01_PLUS);

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 480;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[2][ screenWidth * 10 ];

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    if (tft.getStartCount() == 0)
    {   // Processing if not yet started
        tft.startWrite();
    }
    tft.pushImageDMA( area->x1
                    , area->y1
                    , area->x2 - area->x1 + 1
                    , area->y2 - area->y1 + 1
                    , ( lgfx::swap565_t* )&color_p->full);
    lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;

    data->state = LV_INDEV_STATE_REL;

    if( tft.getTouch( &touchX, &touchY ) )
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

void lvgl_ui()
{
    lv_obj_t* page_main = lv_obj_create(NULL);
    
    lv_obj_t* slider =  lv_slider_create(page_main);
    lv_obj_set_size(slider, 150, 10);
    lv_obj_set_align( slider, LV_ALIGN_CENTER );

    lv_obj_t* label = lv_label_create(page_main);
    lv_label_set_text(label, "Hello LVGL");

    // lv_obj_t* ui_Spinner1 = lv_spinner_create(page_main, 500, 90);
    // lv_obj_set_width(ui_Spinner1, 80);
    // lv_obj_set_height(ui_Spinner1, 80);
    // lv_obj_set_align(ui_Spinner1, LV_ALIGN_CENTER);
    // lv_obj_clear_flag(ui_Spinner1, LV_OBJ_FLAG_CLICKABLE);      /// Flags

    lv_scr_load(page_main);
}

void setup()
{
    tft.begin();

    lv_init();
    lv_disp_draw_buf_init( &draw_buf, buf[0], buf[1], screenWidth * 10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

    lvgl_ui();
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(20);
}