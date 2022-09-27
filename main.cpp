#include "main.hpp"
#include "GlobalNamespace/QuestAppInit.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "extern/includes/bs-utils/shared/utils.hpp"
using namespace GlobalNamespace;
using namespace UnityEngine;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

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
MAKE_HOOK_MATCH(ThiccNotes, &GlobalNamespace::NoteController::Init, void, GlobalNamespace::NoteController* self, GlobalNamespace::NoteData* noteData, float worldRotation, UnityEngine::Vector3 moveStartPos, UnityEngine::Vector3 moveEndPos, UnityEngine::Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity, float endRotation, float uniformScale, bool rotateTowardsPlayer, bool useRandomRotation) {
    
    ThiccNotes(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration,  jumpGravity, endRotation, uniformScale, rotateTowardsPlayer, useRandomRotation);
    self->get_noteTransform()->set_localScale(UnityEngine::Vector3(1.5,1,1));
    bs_utils::Submission::disable(modInfo);
    getLogger().info("disabled score submission");
}

MAKE_HOOK_MATCH(ThiccSaber, &Saber::ManualUpdate, void, Saber *self)
{
    ThiccSaber(self);
        self->get_transform()->set_localScale({2.5, 2, 1});
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

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), ThiccNotes);
    INSTALL_HOOK(getLogger(), ThiccSaber);
    getLogger().info("Installed all hooks!");
}