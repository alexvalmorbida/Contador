#include "JanelaContador.h"
#include <gtkmm/cssprovider.h>
#include <string>
#include <iostream>

// Inicializa estilos CSS para os widgets
void JanelaContador::inicializar_estilos() {
    auto provider = Gtk::CssProvider::create();
    std::string css_data = R"(
        .painel-contador {
            background-color: rgba(0, 0, 0, 0.25);
            border-radius: 12px;
            padding: 20px;
            box-shadow: 0 5px 15px rgba(0, 0, 0, 0.6);
            border: 1px solid rgba(255, 255, 255, 0.1);
        }
    )";
    provider->load_from_data(css_data);
    get_style_context()->add_provider(provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
}


JanelaContador::JanelaContador()
    : m_vbox(Gtk::Orientation::VERTICAL, 15),
    m_button_start_stop("Iniciar"),
    m_button_reset("Reset"),
    m_hbox_botoes(Gtk::Orientation::HORIZONTAL, 10),
    m_hbox_entradas(Gtk::Orientation::HORIZONTAL, 10),
    m_valor_recebido(0)
{
    // Carrega os estilos e define as propriedades da janela
    inicializar_estilos();
	set_title("Contador");     // Título da janela
	set_default_size(600, 400);     // Tamanho padrão da janela
	set_child(m_vbox);              // Define o container principal da janela

    // Configuração dos Widgets
    m_label_contador.set_markup("<span font_desc='Sans Bold 40'>0</span>");

    // Aplicação do Estilo ao Frame
    m_frame_contador.add_css_class("flat");
    m_frame_contador.add_css_class("painel-contador");
    m_frame_contador.set_child(m_label_contador);

    m_frame_contador.set_expand(true);
    m_entry_intervalo.set_text("1000");
    m_entry_incremento.set_text("1");

    //Conexão dos Sinais
    m_button_start_stop.signal_clicked().connect(sigc::mem_fun(*this, &JanelaContador::on_start_stop_clicked));
    m_button_reset.signal_clicked().connect(sigc::mem_fun(*this, &JanelaContador::on_reset_clicked));
    m_dispatcher.connect(sigc::mem_fun(*this, &JanelaContador::notificarAtualizacaoUI));

    // Montagem do Layout da Janela
    m_vbox.set_margin(15);
    m_vbox.append(m_frame_contador);
    m_hbox_entradas.append(*Gtk::make_managed<Gtk::Label>("Intervalo (ms):"));
    m_hbox_entradas.append(m_entry_intervalo);
    m_hbox_entradas.append(*Gtk::make_managed<Gtk::Label>("Incremento:"));
    m_hbox_entradas.append(m_entry_incremento);
    m_vbox.append(m_hbox_entradas);
    m_hbox_botoes.set_halign(Gtk::Align::CENTER);
    m_hbox_botoes.append(m_button_start_stop);
    m_hbox_botoes.append(m_button_reset);
    m_vbox.append(m_hbox_botoes);

    //Inscrição como observador da lógica do contador
    m_contador.adicionarObservador(this);
}


JanelaContador::~JanelaContador() {}

//Click para o botão Iniciar/Parar
void JanelaContador::on_start_stop_clicked() {
    static bool rodando = false;
    if (!rodando) {
        try {
            int intervalo = std::stoi(m_entry_intervalo.get_text());
            int incremento = std::stoi(m_entry_incremento.get_text());
            m_contador.setIntervalo(intervalo);
            m_contador.setIncremento(incremento);
        }
        catch (const std::exception& e) {
            std::cerr << "Entrada inválida: " << e.what() << std::endl;
        }
        m_contador.start();
        m_button_start_stop.set_label("Parar");
    }
    else {
        m_contador.stop();
        m_button_start_stop.set_label("Iniciar");
    }
    rodando = !rodando;
}

//Click para o botão Reset
void JanelaContador::on_reset_clicked() {
    m_contador.reset();
}

// Método chamado pela thread do Contador para notificar uma atualização
void JanelaContador::onContadorAtualizado(long long novoValor) {
    m_valor_recebido = novoValor;
    m_dispatcher.emit();
}

// Método chamado na thread da UI para atualizar o label de forma segura
void JanelaContador::notificarAtualizacaoUI() {
    m_label_contador.set_markup("<span font_desc='Sans Bold 40'>" + std::to_string(m_valor_recebido.load()) + "</span>");
}