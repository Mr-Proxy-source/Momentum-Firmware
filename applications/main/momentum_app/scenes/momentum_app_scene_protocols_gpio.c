#include "../momentum_app.h"

enum VarItemListIndex {
    VarItemListIndexSpiCc1101Handle,
    VarItemListIndexSpiNrf24Handle,
    VarItemListIndexUartEspChannel,
    VarItemListIndexUartNmeaChannel,
    VarItemListIndexUartGeneralChannel,
};

#define SPI_DEFAULT "Default 4"
#define SPI_EXTRA "Extra 7"
#define UART_DEFAULT "Default 13,14"
#define UART_EXTRA "Extra 15,16"

void momentum_app_scene_protocols_gpio_var_item_list_callback(void* context, uint32_t index) {
    MomentumApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

static void momentum_app_scene_protocols_gpio_cc1101_handle_changed(VariableItem* item) {
    MomentumApp* app = variable_item_get_context(item);
    momentum_settings.spi_cc1101_handle =
        variable_item_get_current_value_index(item) == 0 ? SpiDefault : SpiExtra;
    variable_item_set_current_value_text(
        item, momentum_settings.spi_cc1101_handle == SpiDefault ? SPI_DEFAULT : SPI_EXTRA);
    app->save_settings = true;
}

static void momentum_app_scene_protocols_gpio_nrf24_handle_changed(VariableItem* item) {
    MomentumApp* app = variable_item_get_context(item);
    momentum_settings.spi_nrf24_handle =
        variable_item_get_current_value_index(item) == 0 ? SpiDefault : SpiExtra;
    variable_item_set_current_value_text(
        item, momentum_settings.spi_nrf24_handle == SpiDefault ? SPI_DEFAULT : SPI_EXTRA);
    app->save_settings = true;
}

static void momentum_app_scene_protocols_gpio_esp32_channel_changed(VariableItem* item) {
    MomentumApp* app = variable_item_get_context(item);
    momentum_settings.uart_esp_channel = variable_item_get_current_value_index(item) == 0 ?
                                             FuriHalSerialIdUsart :
                                             FuriHalSerialIdLpuart;
    variable_item_set_current_value_text(
        item,
        momentum_settings.uart_esp_channel == FuriHalSerialIdUsart ? UART_DEFAULT : UART_EXTRA);
    app->save_settings = true;
}

static void momentum_app_scene_protocols_gpio_nmea_channel_changed(VariableItem* item) {
    MomentumApp* app = variable_item_get_context(item);
    momentum_settings.uart_nmea_channel = variable_item_get_current_value_index(item) == 0 ?
                                              FuriHalSerialIdUsart :
                                              FuriHalSerialIdLpuart;
    variable_item_set_current_value_text(
        item,
        momentum_settings.uart_nmea_channel == FuriHalSerialIdUsart ? UART_DEFAULT : UART_EXTRA);
    app->save_settings = true;
}

static void momentum_app_scene_protocols_gpio_general_channel_changed(VariableItem* item) {
    MomentumApp* app = variable_item_get_context(item);
    momentum_settings.uart_general_channel = variable_item_get_current_value_index(item) == 0 ?
                                                 FuriHalSerialIdUsart :
                                                 FuriHalSerialIdLpuart;
    variable_item_set_current_value_text(
        item,
        momentum_settings.uart_general_channel == FuriHalSerialIdUsart ? UART_DEFAULT :
                                                                         UART_EXTRA);
    app->save_settings = true;
}

void momentum_app_scene_protocols_gpio_on_enter(void* context) {
    MomentumApp* app = context;
    VariableItemList* var_item_list = app->var_item_list;
    VariableItem* item;

    item = variable_item_list_add(
        var_item_list,
        "CC1101 SPI",
        2,
        momentum_app_scene_protocols_gpio_cc1101_handle_changed,
        app);
    variable_item_set_current_value_index(item, momentum_settings.spi_cc1101_handle);
    variable_item_set_current_value_text(
        item, momentum_settings.spi_cc1101_handle == SpiDefault ? SPI_DEFAULT : SPI_EXTRA);

    item = variable_item_list_add(
        var_item_list, "NRF24 SPI", 2, momentum_app_scene_protocols_gpio_nrf24_handle_changed, app);
    variable_item_set_current_value_index(item, momentum_settings.spi_nrf24_handle);
    variable_item_set_current_value_text(
        item, momentum_settings.spi_nrf24_handle == SpiDefault ? SPI_DEFAULT : SPI_EXTRA);

    item = variable_item_list_add(
        var_item_list,
        "ESP32/8266 UART",
        2,
        momentum_app_scene_protocols_gpio_esp32_channel_changed,
        app);
    variable_item_set_current_value_index(item, momentum_settings.uart_esp_channel);
    variable_item_set_current_value_text(
        item,
        momentum_settings.uart_esp_channel == FuriHalSerialIdUsart ? UART_DEFAULT : UART_EXTRA);

    item = variable_item_list_add(
        var_item_list,
        "NMEA GPS UART",
        2,
        momentum_app_scene_protocols_gpio_nmea_channel_changed,
        app);
    variable_item_set_current_value_index(item, momentum_settings.uart_nmea_channel);
    variable_item_set_current_value_text(
        item,
        momentum_settings.uart_nmea_channel == FuriHalSerialIdUsart ? UART_DEFAULT : UART_EXTRA);

    item = variable_item_list_add(
        var_item_list,
        "General UART",
        2,
        momentum_app_scene_protocols_gpio_general_channel_changed,
        app);
    variable_item_set_current_value_index(item, momentum_settings.uart_general_channel);
    variable_item_set_current_value_text(
        item,
        momentum_settings.uart_general_channel == FuriHalSerialIdUsart ? UART_DEFAULT :
                                                                         UART_EXTRA);

    variable_item_list_set_enter_callback(
        var_item_list, momentum_app_scene_protocols_gpio_var_item_list_callback, app);

    variable_item_list_set_selected_item(
        var_item_list,
        scene_manager_get_scene_state(app->scene_manager, MomentumAppSceneProtocolsGpio));

    view_dispatcher_switch_to_view(app->view_dispatcher, MomentumAppViewVarItemList);
}

bool momentum_app_scene_protocols_gpio_on_event(void* context, SceneManagerEvent event) {
    MomentumApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(
            app->scene_manager, MomentumAppSceneProtocolsGpio, event.event);
        consumed = true;
        switch(event.event) {
        default:
            break;
        }
    }

    return consumed;
}

void momentum_app_scene_protocols_gpio_on_exit(void* context) {
    MomentumApp* app = context;
    variable_item_list_reset(app->var_item_list);
}
