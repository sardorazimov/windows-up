#include "miransas/ui/localization.hpp"

namespace miransas::ui
{
namespace
{
constexpr std::string_view kEnglishAppName = "Miransas Control";
constexpr std::string_view kUzbekAppName = "Miransas Control";
constexpr std::string_view kEnglishWelcome = "Welcome to Miransas Control";
constexpr std::string_view kUzbekWelcome = "Miransas Controlga xush kelibsiz";
constexpr std::string_view kEnglishFeatures = "Features";
constexpr std::string_view kUzbekFeatures = "Imkoniyatlar";
constexpr std::string_view kEnglishPrivacy = "Privacy";
constexpr std::string_view kUzbekPrivacy = "Maxfiylik";
constexpr std::string_view kEnglishToggle = "Switch to Uzbek";
constexpr std::string_view kUzbekToggle = "Inglizcha ga o‘tish";
} // namespace

LocalizationManager::LocalizationManager(Language language) noexcept
    : language_(language)
{
}

void LocalizationManager::set_language(Language language) noexcept
{
    language_ = language;
}

void LocalizationManager::toggle_language() noexcept
{
    language_ = (language_ == Language::English) ? Language::Uzbek : Language::English;
}

Language LocalizationManager::language() const noexcept
{
    return language_;
}

std::string_view LocalizationManager::app_name() const noexcept
{
    return (language_ == Language::English) ? kEnglishAppName : kUzbekAppName;
}

std::string_view LocalizationManager::welcome_message() const noexcept
{
    return (language_ == Language::English) ? kEnglishWelcome : kUzbekWelcome;
}

std::string_view LocalizationManager::features_title() const noexcept
{
    return (language_ == Language::English) ? kEnglishFeatures : kUzbekFeatures;
}

std::string_view LocalizationManager::privacy_title() const noexcept
{
    return (language_ == Language::English) ? kEnglishPrivacy : kUzbekPrivacy;
}

std::string_view LocalizationManager::toggle_label() const noexcept
{
    return (language_ == Language::English) ? kEnglishToggle : kUzbekToggle;
}
} // namespace miransas::ui
