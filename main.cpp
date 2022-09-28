#include "main.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "extern/includes/bs-utils/shared/utils.hpp"
#include "MainConfig.hpp"
#include "questui/shared/QuestUI.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace QuestUI;
using namespace custom_types;


static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup
DEFINE_CONFIG(MainConfig);
// Loads the config from disk using our modInfo, then returns it for use
// other config tools such as config-utils don't use this config, so it can be removed if those are in use
Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation) 
    {
        UnityEngine::GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());
        QuestUI::BeatSaberUI::CreateText(container->get_transform(), "Enable Thicc Mod here");
        UnityEngine::UI::Toggle* isEnabled = AddConfigValueToggle(container->get_transform(), getMainConfig().isEnabled);
        QuestUI::BeatSaberUI::AddHoverHint(isEnabled->get_gameObject(), "Enable Thicc Mod?");
        UnityEngine::UI::Toggle* megaThicc = AddConfigValueToggle(container->get_transform(), getMainConfig().megaThicc);
        QuestUI::BeatSaberUI::AddHoverHint(megaThicc->get_gameObject(), "THICC AS FUCK");
    }
}   

MAKE_HOOK_MATCH(ThiccNotes, &GlobalNamespace::NoteController::Init, void, GlobalNamespace::NoteController* self, GlobalNamespace::NoteData* noteData, float worldRotation, UnityEngine::Vector3 moveStartPos, UnityEngine::Vector3 moveEndPos, UnityEngine::Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity, float endRotation, float uniformScale, bool rotateTowardsPlayer, bool useRandomRotation) {
    
    ThiccNotes(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration,  jumpGravity, endRotation, uniformScale, rotateTowardsPlayer, useRandomRotation);
    if(getMainConfig().isEnabled.GetValue() & getMainConfig().megaThicc.GetValue() == true){
        self->get_noteTransform()->set_localScale(UnityEngine::Vector3(3,1,1));
        getLogger().info("your shit be thicc as fuck now ;)");
    }
    else if(getMainConfig().isEnabled.GetValue() == true){
        self->get_noteTransform()->set_localScale(UnityEngine::Vector3(1.5,1,1));
        getLogger().info("your shit be thicc now");
    }
    else if (getMainConfig().isEnabled.GetValue() == false)
    {
        bs_utils::Submission::enable(modInfo);
        getLogger().info("no longer thicc :(");
    }
    
}

MAKE_HOOK_MATCH(MainMenuUIHook, &GlobalNamespace::MainMenuViewController::DidActivate, void, GlobalNamespace::MainMenuViewController
*self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {

    MainMenuUIHook(self, firstActivation, addedToHierarchy, screenSystemEnabling); 
    
    if(getMainConfig().isEnabled.GetValue() == true){
        bs_utils::Submission::disable(modInfo);
        getLogger().info("disabled score submission due to thicc mod");
    }
    else if (getMainConfig().isEnabled.GetValue() == false)
    {
        bs_utils::Submission::enable(modInfo);
        getLogger().info("enabled score submission due to thicc mod being disabled");
    }
    
}

MAKE_HOOK_MATCH(ThiccSaber, &Saber::ManualUpdate, void, Saber *self)
{
    if(getMainConfig().isEnabled.GetValue() & getMainConfig().megaThicc.GetValue() == true){
        ThiccSaber(self);
        self->get_transform()->set_localScale({3.5, 3, 1});
    }
    else if(getMainConfig().isEnabled.GetValue() == true){
        ThiccSaber(self);
        self->get_transform()->set_localScale({2.5, 2, 1});    
    }
    else if (getMainConfig().isEnabled.GetValue() == false)
    {
        ThiccSaber(self);
        self->get_transform()->set_localScale({1, 1, 1});
    }
    
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;
    getConfig().Load();
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    getMainConfig().Init(modInfo);
    QuestUI::Init();
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);
    QuestUI::Register::RegisterMainMenuModSettingsViewController(modInfo, DidActivate);
    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), ThiccNotes);
    INSTALL_HOOK(getLogger(), ThiccSaber);
    INSTALL_HOOK(getLogger(), MainMenuUIHook);
    getLogger().info("Installed all hooks!");
}
