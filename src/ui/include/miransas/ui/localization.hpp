#pragma once

#include <string_view>

namespace miransas::ui
{
enum class Language
{
    English,
    Uzbek
};

class LocalizationManager
{
public:
    explicit LocalizationManager(Language language = Language::English) noexcept;

    void set_language(Language language) noexcept;
    void toggle_language() noexcept;

    Language language() const noexcept;

    std::string_view app_name() const noexcept;
    std::string_view welcome_message() const noexcept;
    std::string_view features_title() const noexcept;
    std::string_view privacy_title() const noexcept;
    std::string_view toggle_label() const noexcept;

private:
    Language language_;
};
} // namespace miransas::ui
