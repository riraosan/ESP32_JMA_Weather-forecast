
#pragma once

#include <M5GFX.h>

class LGFX_8BIT_CVBS : public lgfx::LGFX_Device {
 public:
  lgfx::Panel_CVBS _panel_instance;

  LGFX_8BIT_CVBS(void) {
    {                                       // Configure display panel control settings
      auto cfg = _panel_instance.config();  // Gets the structure for display panel settings

      // output resolution settings
      cfg.memory_width  = 320;  // Output Resolution Width
      cfg.memory_height = 240;  // Output resolution Height
      // the actual resolution to be used settings
      cfg.panel_width  = 320 - 8;   // Actual width   (Set a value equal to or smaller than memory_width)
      cfg.panel_height = 240 - 16;  // Actual height  (Set a value equal to or smaller than memory_height)

      // the amount of display position offset settings
      cfg.offset_x = 4;  // Amount to shift the display position to the right (initial value 0)
      cfg.offset_y = 8;  // Amount to shift the display position down (initial value 0)

      _panel_instance.config(cfg);

      // Normally, you should specify the same values for memory_width and panel_width and use offset_x = 0
      // If you want to prevent the edge of the screen from being hidden, set the value of panel_width smaller than memory_width and adjust the left/right position with offset_x.
      // For example, if you set panel_width to 32 less than memory_width, you can set offset_x to 16 to center the left and right positions.
      // You should do the same for the vertical direction (memory_height , panel_height , offset_y ) and adjust as needed.
    }

    {
      auto cfg = _panel_instance.config_detail();

      // output signal type settings
      cfg.signal_type = cfg.signal_type_t::NTSC_J;

      // GPIO number of output destination settings
      cfg.pin_dac = 26;  // For using DAC, you can choose only 25 or 26

      // PSRAM memory allocation settings
      cfg.use_psram = 1;  // 0=PSRAM not used / 1=Half PSRAM and half SRAM / 2=All using PSRAM

      // the amplitude strength of the output signal settings
      // ※ If the signal is attenuated because the GPIO has a protective resistor or for some other reason, you should increase the value.
      // ※ Since M5StackCore2 has protection resistors on the GPIOs, we recommend 200.
      cfg.output_level = 128;  // Initial value 128

      // the strength of the amplitude of the saturation signal settings
      // Decrease the value to decrease saturation, and set to 0 for black and white. Increasing the value increases saturation.
      cfg.chroma_level = 128;  // Initial value 128

      _panel_instance.config_detail(cfg);
    }

    setPanel(&_panel_instance);
  }
};
