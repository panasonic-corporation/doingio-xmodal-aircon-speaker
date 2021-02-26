const int status_txt_posx = 100;
const int status_txt_posy = 90;
const int btn_width = 130;
const int btn_height= 80;
const int btn_posy = 130;

class UIController{
private:

public:
    static void updateMainUI(int command_status);
    static void updateGainUI(int command_status, float gain);
    static void updateButtonUI(int command_status, bool stop_flag);
};