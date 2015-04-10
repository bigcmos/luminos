struct RGBColor {
  uint8_t r, g, b;
};

// TODO(daden): Add an intensity field that is the magnitude of the RGB color
// vector and each color is based off of percentages of that intensity.
RGBColor RGB_ORANGE = RGBColor {160, 40, 0};
RGBColor RGB_GREEN = RGBColor{128, 255, 0}; // Halloween
RGBColor RGB_PURPLE = RGBColor{150, 0, 150};
RGBColor RGB_RED = RGBColor{255, 0, 0};
RGBColor RGB_BLUE = RGBColor{0, 0, 255};
RGBColor RGB_WHITE = RGBColor{255, 255, 255};
