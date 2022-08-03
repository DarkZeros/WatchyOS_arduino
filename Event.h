
enum class Event : uint8_t {
    WAKEUP_EXT0,
    WAKEUP_EXT1,
    WAKEUP_TIMER,

    RTC_INT,
    BMA_INT,

    INT_RTC,
    INT_BMA,

    BTN_MENU,
    BTN_BACK,
    BTN_DOWN,
    BTN_UP,

    WATCHFACE,
};