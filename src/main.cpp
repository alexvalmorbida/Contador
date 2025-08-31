#include "JanelaContador.h"
#include <gtkmm/application.h>


int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.contador.teste.gertech");
    auto settings = Gtk::Settings::get_default();
    if (settings) {
        // Define a propriedade para preferir o tema escuro
        settings->set_property("gtk-application-prefer-dark-theme", true);
    }
    return app->make_window_and_run<JanelaContador>(argc, argv);
}