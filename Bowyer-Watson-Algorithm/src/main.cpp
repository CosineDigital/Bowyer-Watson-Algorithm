#include "application.h"

int main()
{
    Application mainApp(500, 250, "Bowyer Watson Algorithm", framebuffer_size_callback);
    mainApp.start();
    return 0;
}
