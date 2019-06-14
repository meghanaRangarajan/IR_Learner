extern "C" void enable();
extern "C" void pin_callback_entry(int gpio, int level, uint32_t tick);
extern "C" void shutdown();
extern "C" void disable();
extern "C" int analyse_ir_pulses();
extern "C" int decode_ir_sequence();
extern "C" uint32_t code();
extern "C" void init(int pin);
extern "C" int create_insert(int key,uint32_t code);

extern "C" uint32_t _try_decode_nec(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_rc5(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_rc6(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_rcmm(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_xmp1(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_rc5_57(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_nec_short(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_sony(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_panasonic(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_jvc(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_rc5_38(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_sharp(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_rca_38(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_rca_57(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_mitsubishi(std::vector<std::pair<int,int>> sequence);
extern "C" uint32_t _try_decode_konka(std::vector<std::pair<int,int>> sequence);
