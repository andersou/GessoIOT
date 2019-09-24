void temp2rgb(unsigned int kelvin, int *red, int *green, int *blue) {
  int temp = kelvin / 100;
  int r, g, b;

  if (temp <= 66) {
    r = 255;
    g = 99.4708025861 * log(temp) - 161.1195681661;
    if (temp <= 19) {
      b = 0;
    } else {
      b = 138.5177312231 * log(temp - 10) - 305.0447927307;
    }
  } else {
    if (temp < 60) {
      r = 0;
      g = 0;
    } else {
      r = 329.698727446 * pow(temp - 60, -0.1332047592);
      g = 288.1221695283 * pow(temp - 60, -0.0755148492);
    }
    b = 255;
  }

  r = (r > 0) ? r : 0;
  g = (g > 0) ? g : 0;
  b = (b > 0) ? b : 0;
  *red = r;
  *green = g;
  *blue = b;
  return;
}
