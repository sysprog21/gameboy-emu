#include <stdint.h>

/* Internal function used to read bytes. */
static uint8_t __gb_read(struct gb_s *gb, const uint_fast16_t addr);

/* Internal function used to write bytes */
static void __gb_write(struct gb_s *gb,
                       const uint_fast16_t addr,
                       const uint8_t val);
                       
static uint8_t __gb_execute_cb(struct gb_s *gb);

static void __gb_step_cpu(struct gb_s *gb);

#if ENABLE_LCD
static void __gb_draw_line(struct gb_s *gb);
#endif
