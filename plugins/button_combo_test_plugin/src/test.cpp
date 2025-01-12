#include <catch2/catch_amalgamated.hpp>

#include <wups/button_combo/api.h>

void stubCallback(WUPSButtonCombo_ControllerTypes, WUPSButtonCombo_ComboHandle, void *) {
}
void stubCallback2(WUPSButtonCombo_ControllerTypes, WUPSButtonCombo_ComboHandle, void *) {
}
static void *DEFAULT_CALLBACK_CONTEXT = reinterpret_cast<void *>(0x13371337);
constexpr auto BASE_COMBO             = WUPS_BUTTON_COMBO_BUTTON_RESERVED_BIT | WUPS_BUTTON_COMBO_BUTTON_ZL | WUPS_BUTTON_COMBO_BUTTON_ZR;
static std::string BASE_LABEL         = "label";

TEST_CASE("Creating a hold combination works") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const auto resOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            BASE_COMBO,
            1,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            status,
            error);

    REQUIRE(resOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    {
        WUPSButtonCombo_ButtonComboInfoEx info;
        const auto infoRes = resOpt->GetButtonComboInfoEx(info);
        REQUIRE(infoRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(info.type == WUPS_BUTTON_COMBO_COMBO_TYPE_HOLD);
        REQUIRE(info.optionalHoldForXMs == 1);
        REQUIRE(info.basicCombo.combo == BASE_COMBO);
        REQUIRE(info.basicCombo.controllerMask == WUPS_BUTTON_COMBO_CONTROLLER_ALL);
    }
}

TEST_CASE("Creating a press down combination works") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;

    const auto resOpt = WUPSButtonComboAPI::CreateComboPressDown(
            BASE_LABEL,
            BASE_COMBO,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            status,
            error);

    REQUIRE(resOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        WUPSButtonCombo_ButtonComboInfoEx info;
        const auto infoRes = resOpt->GetButtonComboInfoEx(info);
        REQUIRE(infoRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(info.type == WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN);
        REQUIRE(info.optionalHoldForXMs == 0);
        REQUIRE(info.basicCombo.combo == BASE_COMBO);
        REQUIRE(info.basicCombo.controllerMask == WUPS_BUTTON_COMBO_CONTROLLER_ALL);
    }
}

TEST_CASE("Creating a hold observer combination works") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const auto resOpt = WUPSButtonComboAPI::CreateComboHoldObserver(
            BASE_LABEL,
            BASE_COMBO,
            1,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            status,
            error);

    REQUIRE(resOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    {
        WUPSButtonCombo_ButtonComboInfoEx info;
        const auto infoRes = resOpt->GetButtonComboInfoEx(info);
        REQUIRE(infoRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(info.type == WUPS_BUTTON_COMBO_COMBO_TYPE_HOLD_OBSERVER);
        REQUIRE(info.optionalHoldForXMs == 1);
        REQUIRE(info.basicCombo.combo == BASE_COMBO);
        REQUIRE(info.basicCombo.controllerMask == WUPS_BUTTON_COMBO_CONTROLLER_ALL);
    }
}

TEST_CASE("Creating a press down observer combination works") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;


    const auto resOpt = WUPSButtonComboAPI::CreateComboPressDownObserver(
            BASE_LABEL,
            BASE_COMBO,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            status,
            error);

    REQUIRE(resOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        WUPSButtonCombo_ButtonComboInfoEx info;
        const auto infoRes = resOpt->GetButtonComboInfoEx(info);
        REQUIRE(infoRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(info.type == WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN_OBSERVER);
        REQUIRE(info.optionalHoldForXMs == 0);
        REQUIRE(info.basicCombo.combo == BASE_COMBO);
        REQUIRE(info.basicCombo.controllerMask == WUPS_BUTTON_COMBO_CONTROLLER_ALL);
    }
}

TEST_CASE("Creating a hold ex combination works") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;

    const auto resOpt = WUPSButtonComboAPI::CreateComboHoldEx(
            BASE_LABEL,
            WUPS_BUTTON_COMBO_CONTROLLER_WPAD_0,
            BASE_COMBO,
            2,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            false,
            status,
            error);

    REQUIRE(resOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        WUPSButtonCombo_ButtonComboInfoEx info;
        const auto infoRes = resOpt->GetButtonComboInfoEx(info);
        REQUIRE(infoRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(info.type == WUPS_BUTTON_COMBO_COMBO_TYPE_HOLD);
        REQUIRE(info.optionalHoldForXMs == 2);
        REQUIRE(info.basicCombo.combo == BASE_COMBO);
        REQUIRE(info.basicCombo.controllerMask == WUPS_BUTTON_COMBO_CONTROLLER_WPAD_0);
    }
}

TEST_CASE("Creating a press down ex combination works") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;

    const auto resOpt = WUPSButtonComboAPI::CreateComboPressDownEx(
            BASE_LABEL,
            WUPS_BUTTON_COMBO_CONTROLLER_WPAD_1,
            BASE_COMBO,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            true,
            status,
            error);

    REQUIRE(resOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        WUPSButtonCombo_ButtonComboInfoEx info;
        const auto infoRes = resOpt->GetButtonComboInfoEx(info);
        REQUIRE(infoRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(info.type == WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN_OBSERVER);
        REQUIRE(info.optionalHoldForXMs == 0);
        REQUIRE(info.basicCombo.combo == BASE_COMBO);
        REQUIRE(info.basicCombo.controllerMask == WUPS_BUTTON_COMBO_CONTROLLER_WPAD_1);
    }
}

TEST_CASE("Creating a combo fails if combo is empty") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;

    const auto resOpt = WUPSButtonComboAPI::CreateComboPressDown(
            BASE_LABEL,
            static_cast<WUPSButtonCombo_Buttons>(0),
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            status,
            error);

    REQUIRE(!resOpt.has_value());
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS);
}

TEST_CASE("Creating a combo still works if the label is empty") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;

    const auto resOpt = WUPSButtonComboAPI::CreateComboPressDown(
            std::string_view(""),
            BASE_COMBO,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            status,
            error);

    REQUIRE(resOpt.has_value());
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
}

TEST_CASE("Creating a combo works if context is empty") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;

    const auto resOpt = WUPSButtonComboAPI::CreateComboPressDown(
            BASE_LABEL,
            BASE_COMBO,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            status,
            error);

    REQUIRE(resOpt.has_value());
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
}

TEST_CASE("Creating a hold fails if hold duration is 0") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const auto resOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            BASE_COMBO,
            0,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            status,
            error);

    REQUIRE(!resOpt.has_value());
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS);
}

TEST_CASE("Different combos are no conflict") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    // Register Combo on all controllers
    const auto noConflictOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            BASE_COMBO,
            1,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            status,
            error);

    REQUIRE(noConflictOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    { // create a different hold button combo
        const auto otherOpt = WUPSButtonComboAPI::CreateComboHold(
                BASE_LABEL,
                WUPS_BUTTON_COMBO_BUTTON_RESERVED_BIT | WUPS_BUTTON_COMBO_BUTTON_1,
                1,
                stubCallback,
                DEFAULT_CALLBACK_CONTEXT,
                status,
                error);

        REQUIRE(otherOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    { // create a different press down button combo
        const auto otherOpt = WUPSButtonComboAPI::CreateComboPressDown(
                BASE_LABEL,
                WUPS_BUTTON_COMBO_BUTTON_RESERVED_BIT | WUPS_BUTTON_COMBO_BUTTON_2,
                stubCallback,
                DEFAULT_CALLBACK_CONTEXT,
                status,
                error);

        REQUIRE(otherOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
}

TEST_CASE("Detecting a conflict is working") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const WUPSButtonCombo_Buttons baseCombo = BASE_COMBO;
    // Register Combo on all controllers
    const auto noConflictOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            baseCombo,
            1,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            status,
            error);

    REQUIRE(noConflictOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        // Conflict 1a; button that's already used in another combo
        const auto conflictOpt = WUPSButtonComboAPI::CreateComboHold(
                BASE_LABEL,
                baseCombo & ~WUPS_BUTTON_COMBO_BUTTON_ZL,
                1,
                stubCallback,
                DEFAULT_CALLBACK_CONTEXT,
                status,
                error);

        REQUIRE(conflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        // Conflict 1b; button that's already used in another combo by other combo type
        const auto conflictOpt = WUPSButtonComboAPI::CreateComboPressDown(
                BASE_LABEL,
                baseCombo & ~WUPS_BUTTON_COMBO_BUTTON_ZL,
                stubCallback,
                DEFAULT_CALLBACK_CONTEXT,
                status,
                error);

        REQUIRE(conflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        // Conflict 2a; Same button as existing combo
        const auto conflictOpt = WUPSButtonComboAPI::CreateComboHold(
                BASE_LABEL,
                baseCombo,
                1,
                stubCallback,
                DEFAULT_CALLBACK_CONTEXT,
                status,
                error);

        REQUIRE(conflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        // Conflict 2b; Same button as existing combo; but other combo type
        const auto conflictOpt = WUPSButtonComboAPI::CreateComboPressDown(
                BASE_LABEL,
                baseCombo,
                stubCallback,
                DEFAULT_CALLBACK_CONTEXT,
                status,
                error);

        REQUIRE(conflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        // Conflict 3a: includes existing combo
        const auto conflictOpt = WUPSButtonComboAPI::CreateComboHold(
                BASE_LABEL,
                baseCombo | WUPS_BUTTON_COMBO_BUTTON_L,
                1,
                stubCallback,
                DEFAULT_CALLBACK_CONTEXT,
                status,
                error);

        REQUIRE(conflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        // Conflict 3b: includes existing combo; but other combo type
        const auto conflictOpt = WUPSButtonComboAPI::CreateComboPressDown(
                BASE_LABEL,
                baseCombo | WUPS_BUTTON_COMBO_BUTTON_L,
                stubCallback,
                DEFAULT_CALLBACK_CONTEXT,
                status,
                error);

        REQUIRE(conflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        // Conflict 4a; just on one controller
        const auto conflictOpt = WUPSButtonComboAPI::CreateComboHoldEx(
                BASE_LABEL,
                WUPS_BUTTON_COMBO_CONTROLLER_VPAD_0,
                baseCombo,
                1,
                stubCallback,
                DEFAULT_CALLBACK_CONTEXT,
                false,
                status,
                error);

        REQUIRE(conflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        // Conflict 4b; just on one controller; but other combo type
        const auto conflictOpt = WUPSButtonComboAPI::CreateComboPressDownEx(
                BASE_LABEL,
                WUPS_BUTTON_COMBO_CONTROLLER_VPAD_0,
                baseCombo,
                stubCallback,
                DEFAULT_CALLBACK_CONTEXT,
                false,
                status,
                error);

        REQUIRE(conflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
}

TEST_CASE("No conflict when using the same combo on multiple controllers") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const WUPSButtonCombo_Buttons baseCombo = BASE_COMBO;
    // Register Combo on all controllers
    const auto baseComboOpt = WUPSButtonComboAPI::CreateComboHoldEx(
            BASE_LABEL,
            WUPS_BUTTON_COMBO_CONTROLLER_VPAD_0,
            baseCombo,
            1,
            stubCallback,
            nullptr,
            false,
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        // No conflict because it's only for VPAD1, not VPAD0
        const auto noConflictOpt = WUPSButtonComboAPI::CreateComboHoldEx(
                BASE_LABEL,
                WUPS_BUTTON_COMBO_CONTROLLER_VPAD_1,
                baseCombo,
                1,
                stubCallback,
                nullptr,
                false,
                status,
                error);

        REQUIRE(noConflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        // No conflict because it's all controllers but VPAD 0
        const auto noConflictOpt = WUPSButtonComboAPI::CreateComboHoldEx(
                BASE_LABEL,
                WUPS_BUTTON_COMBO_CONTROLLER_ALL & ~WUPS_BUTTON_COMBO_CONTROLLER_VPAD_0,
                baseCombo,
                1,
                stubCallback,
                nullptr,
                false,
                status,
                error);

        REQUIRE(noConflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
}

TEST_CASE("Observer won't trigger a conflict") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const WUPSButtonCombo_Buttons baseCombo = BASE_COMBO;
    // Register Combo on all controllers
    const auto baseComboOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            baseCombo,
            1,
            stubCallback,
            nullptr,
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        // No conflict because it's just a hold observer
        const auto noConflictOpt = WUPSButtonComboAPI::CreateComboHoldObserver(
                BASE_LABEL,
                baseCombo,
                1,
                stubCallback,
                nullptr,
                status,
                error);

        REQUIRE(noConflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        // No conflict because it's just a press down observer
        const auto noConflictOpt = WUPSButtonComboAPI::CreateComboPressDownObserver(
                BASE_LABEL,
                baseCombo,
                stubCallback,
                nullptr,
                status,
                error);

        REQUIRE(noConflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
}

TEST_CASE("Conflict but we resolve it by updating the combo") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const WUPSButtonCombo_Buttons baseCombo = BASE_COMBO;
    // Register Combo on all controllers
    const auto baseComboOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            baseCombo,
            1,
            stubCallback,
            nullptr,
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        const auto conflictAtFirstOpt = WUPSButtonComboAPI::CreateComboHold(
                BASE_LABEL,
                baseCombo,
                1,
                stubCallback,
                nullptr,
                status,
                error);

        REQUIRE(conflictAtFirstOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

        constexpr auto newCombo = WUPS_BUTTON_COMBO_BUTTON_STICK_R | WUPS_BUTTON_COMBO_BUTTON_RESERVED_BIT;
        {
            const auto updateRes = conflictAtFirstOpt->UpdateButtonCombo(newCombo, status);
            REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
            REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
        }
        {
            WUPSButtonCombo_ButtonComboInfoEx info;
            auto updateRes = conflictAtFirstOpt->GetButtonComboInfoEx(info);
            REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
            REQUIRE(info.basicCombo.combo == newCombo);
        }
    }
}

TEST_CASE("Conflict but we resolve it by updating the controller mask") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const WUPSButtonCombo_Buttons baseCombo = BASE_COMBO;
    // Register Combo on all controllers
    const auto baseComboOpt = WUPSButtonComboAPI::CreateComboHoldEx(
            BASE_LABEL,
            WUPS_BUTTON_COMBO_CONTROLLER_VPAD_0,
            baseCombo,
            1,
            stubCallback,
            nullptr,
            false,
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        const auto conflictAtFirstOpt = WUPSButtonComboAPI::CreateComboHoldEx(
                BASE_LABEL,
                WUPS_BUTTON_COMBO_CONTROLLER_VPAD_0,
                baseCombo,
                1,
                stubCallback,
                nullptr,
                false,
                status,
                error);

        REQUIRE(conflictAtFirstOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

        {
            auto updateRes = conflictAtFirstOpt->UpdateControllerMask(WUPS_BUTTON_COMBO_CONTROLLER_VPAD_1, status);
            REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
            REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
        }
        {
            WUPSButtonCombo_ButtonComboInfoEx info;
            auto updateRes = conflictAtFirstOpt->GetButtonComboInfoEx(info);
            REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
            REQUIRE(info.basicCombo.controllerMask == WUPS_BUTTON_COMBO_CONTROLLER_VPAD_1);
        }
    }
}

TEST_CASE("Conflict stays until combo or controller updated") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    // Register Combo on all controllers
    auto baseComboOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            BASE_COMBO,
            1,
            stubCallback,
            nullptr,
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    const auto otherComboOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            BASE_COMBO,
            1,
            stubCallback,
            nullptr,
            status,
            error);

    REQUIRE(otherComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    // Delete first combo
    baseComboOpt.reset();

    // Confirm the combo status of the other combo is conflict
    {
        WUPSButtonCombo_ComboStatus statusFromApi;
        auto updateRes = otherComboOpt->GetButtonComboStatus(statusFromApi);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(statusFromApi == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
    }
    // Update to the existing combo, but this will re check the state
    {
        const auto updateRes = otherComboOpt->UpdateButtonCombo(BASE_COMBO, status);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    }
    // Confirm it's valid now
    {
        WUPSButtonCombo_ComboStatus statusFromApi;
        auto updateRes = otherComboOpt->GetButtonComboStatus(statusFromApi);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(statusFromApi == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    }
}

TEST_CASE("CheckComboAvailable works for WUPS_BUTTON_COMBO_CONTROLLER_ALL mask") {
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL;
        WUPSButtonCombo_ComboStatus status;
        const auto res = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }


    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    // Register Combo on all controllers
    auto baseComboOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            BASE_COMBO,
            1,
            stubCallback,
            nullptr,
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    // Should not be available anymore
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL;
        const auto res         = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
    }

    // remove button combo
    baseComboOpt.reset();

    // Should be available again
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL;
        const auto res         = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    }

    // if we add an observer, it should still be available

    // Register Combo on all controllers
    baseComboOpt = WUPSButtonComboAPI::CreateComboHoldObserver(
            BASE_LABEL,
            BASE_COMBO,
            1,
            stubCallback,
            nullptr,
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    // Should be still available
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL;
        const auto res         = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    }
}

TEST_CASE("CheckComboAvailable works for mixed controller mask") {
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL;
        WUPSButtonCombo_ComboStatus status;
        const auto res = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }

    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    // Register Combo on all controllers
    auto baseComboOpt = WUPSButtonComboAPI::CreateComboHoldEx(
            BASE_LABEL,
            WUPS_BUTTON_COMBO_CONTROLLER_WPAD_0,
            BASE_COMBO,
            1,
            stubCallback,
            nullptr,
            false,
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    // Should not be available anymore for WUPS_BUTTON_COMBO_CONTROLLER_ALL
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL;
        const auto res         = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
    }

    // Should  be available for any controller but WPAD_0
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL & ~WUPS_BUTTON_COMBO_CONTROLLER_WPAD_0;
        const auto res         = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    }

    // Change controller mask to WPAD, so we are only listing to WPAD controllers
    {
        const auto res = baseComboOpt->UpdateControllerMask(WUPS_BUTTON_COMBO_CONTROLLER_WPAD, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    }

    // Should be available for any vpad controller
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_VPAD;
        const auto res         = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    }

    // But not any WPAD
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_VPAD | WUPS_BUTTON_COMBO_CONTROLLER_WPAD_0;
        const auto res         = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
    }

    // Also not a single WPAD
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_WPAD_3;
        const auto res         = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
    }
}

TEST_CASE("Adding observer won't affect CheckComboAvailable result") {
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL;
        WUPSButtonCombo_ComboStatus status;
        const auto res = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }

    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    // Register Combo on all controllers
    auto baseComboOpt = WUPSButtonComboAPI::CreateComboHoldObserver(
            BASE_LABEL,
            BASE_COMBO,
            1,
            stubCallback,
            nullptr,
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    // Should be still available for WUPS_BUTTON_COMBO_CONTROLLER_ALL
    {
        WUPSButtonCombo_ButtonComboOptions options;
        options.combo          = BASE_COMBO;
        options.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL;
        const auto res         = WUPSButtonComboAPI::CheckComboAvailable(options, status);
        REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    }
}

TEST_CASE("UpdateButtonComboCallback and GetButtonComboCallback works") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const WUPSButtonCombo_Buttons baseCombo = BASE_COMBO;
    // Register Combo on all controllers
    const auto baseComboOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            baseCombo,
            1,
            stubCallback,
            reinterpret_cast<void *>(0x13371337),
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        WUPSButtonCombo_CallbackOptions callbackOptions;
        auto updateRes = baseComboOpt->GetButtonComboCallback(callbackOptions);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(callbackOptions.callback == stubCallback);
        REQUIRE(callbackOptions.context == reinterpret_cast<void *>(0x13371337));
    }

    {
        WUPSButtonCombo_CallbackOptions callbackOptions;
        callbackOptions.callback = stubCallback2;
        callbackOptions.context  = reinterpret_cast<void *>(0x42424242);
        auto updateRes           = baseComboOpt->UpdateButtonComboCallback(callbackOptions);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        WUPSButtonCombo_CallbackOptions callbackOptions;
        auto updateRes = baseComboOpt->GetButtonComboCallback(callbackOptions);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(callbackOptions.callback == stubCallback2);
        REQUIRE(callbackOptions.context == reinterpret_cast<void *>(0x42424242));
    }
}

TEST_CASE("GetButtonComboStatus works") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const WUPSButtonCombo_Buttons baseCombo = BASE_COMBO;
    // Register Combo on all controllers
    const auto baseComboOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            baseCombo,
            1,
            stubCallback,
            reinterpret_cast<void *>(0x13371337),
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        WUPSButtonCombo_ComboStatus statusFromApi;
        auto updateRes = baseComboOpt->GetButtonComboStatus(statusFromApi);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(statusFromApi == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    }

    {
        // CONFLICT!
        const auto conflictOpt = WUPSButtonComboAPI::CreateComboHold(
                BASE_LABEL,
                baseCombo,
                1,
                stubCallback,
                nullptr,
                status,
                error);

        REQUIRE(conflictOpt.has_value());
        REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        {
            WUPSButtonCombo_ComboStatus statusFromApi;
            auto updateRes = conflictOpt->GetButtonComboStatus(statusFromApi);
            REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
            REQUIRE(statusFromApi == WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT);
        }
    }
}

TEST_CASE("UpdateButtonComboMeta and GetButtonComboMeta works") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const WUPSButtonCombo_Buttons baseCombo = BASE_COMBO;
    // Register Combo on all controllers
    const auto baseComboOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            baseCombo,
            1,
            stubCallback,
            reinterpret_cast<void *>(0x13371337),
            status,
            error);

    REQUIRE(baseComboOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);

    {
        WUPSButtonComboAPI::ButtonCombo::MetaOptions metaOptionsOut;
        auto updateRes = baseComboOpt->GetButtonComboMeta(metaOptionsOut);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(metaOptionsOut.label == BASE_LABEL);
    }

    const auto newLabel = "new label";
    {
        WUPSButtonComboAPI::ButtonCombo::MetaOptions metaOptions{.label = newLabel};
        auto updateRes = baseComboOpt->UpdateButtonComboMeta(metaOptions);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        WUPSButtonComboAPI::ButtonCombo::MetaOptions metaOptionsOut;
        auto updateRes = baseComboOpt->GetButtonComboMeta(metaOptionsOut);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(metaOptionsOut.label == newLabel);
    }
    // Setting it to empty label still works;
    {
        WUPSButtonComboAPI::ButtonCombo::MetaOptions metaOptions{.label = ""};
        auto updateRes = baseComboOpt->UpdateButtonComboMeta(metaOptions);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        WUPSButtonComboAPI::ButtonCombo::MetaOptions metaOptionsOut;
        auto updateRes = baseComboOpt->GetButtonComboMeta(metaOptionsOut);
        REQUIRE(updateRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(metaOptionsOut.label == "");
    }
}

TEST_CASE("Updating hold duration works") {
    WUPSButtonCombo_ComboStatus status;
    WUPSButtonCombo_Error error;
    const auto resOpt = WUPSButtonComboAPI::CreateComboHold(
            BASE_LABEL,
            BASE_COMBO,
            1,
            stubCallback,
            DEFAULT_CALLBACK_CONTEXT,
            status,
            error);

    REQUIRE(resOpt.has_value());
    REQUIRE(status == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID);
    REQUIRE(error == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    {
        WUPSButtonCombo_ButtonComboInfoEx info;
        const auto infoRes = resOpt->GetButtonComboInfoEx(info);
        REQUIRE(infoRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(info.optionalHoldForXMs == 1);
    }
    constexpr auto newHoldDurationMs = 500;
    {
        const auto infoRes = resOpt->UpdateHoldDuration(newHoldDurationMs);
        REQUIRE(infoRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    }
    {
        WUPSButtonCombo_ButtonComboInfoEx info;
        const auto infoRes = resOpt->GetButtonComboInfoEx(info);
        REQUIRE(infoRes == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
        REQUIRE(info.optionalHoldForXMs == newHoldDurationMs);
    }
}

TEST_CASE("Passing invalid args to AddButtonCombo fails") {
    const auto res = WUPSButtonComboAPI_AddButtonCombo(nullptr, nullptr, nullptr);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS);
}


static WUPSButtonCombo_ComboOptions getDefaultComboOptions() {
    WUPSButtonCombo_ComboOptions options;
    options.callbackOptions.callback                     = stubCallback;
    options.callbackOptions.context                      = DEFAULT_CALLBACK_CONTEXT;
    options.metaOptions.label                            = BASE_LABEL.c_str();
    options.buttonComboOptions.type                      = WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN;
    options.buttonComboOptions.basicCombo.combo          = BASE_COMBO;
    options.buttonComboOptions.basicCombo.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL;
    options.buttonComboOptions.optionalHoldForXMs        = 0;
    return options;
}

TEST_CASE("WUPSButtonComboAPI_AddButtonCombo works") {
    const WUPSButtonCombo_ComboOptions options = getDefaultComboOptions();
    WUPSButtonCombo_ComboHandle handle;
    WUPSButtonCombo_ComboStatus status;
    auto res = WUPSButtonComboAPI_AddButtonCombo(&options, &handle, &status);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    res = WUPSButtonComboAPI_RemoveButtonCombo(handle);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
}

TEST_CASE("WUPSButtonComboAPI_AddButtonCombo fails if handle is nullptr") {
    const WUPSButtonCombo_ComboOptions options = getDefaultComboOptions();
    WUPSButtonCombo_ComboHandle handle;
    WUPSButtonCombo_ComboStatus status;
    auto res = WUPSButtonComboAPI_AddButtonCombo(&options, nullptr, &status);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS);
}

TEST_CASE("WUPSButtonComboAPI_AddButtonCombo works if status is nullptr") {
    const WUPSButtonCombo_ComboOptions options = getDefaultComboOptions();
    WUPSButtonCombo_ComboHandle handle;
    auto res = WUPSButtonComboAPI_AddButtonCombo(&options, &handle, nullptr);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
    res = WUPSButtonComboAPI_RemoveButtonCombo(handle);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_SUCCESS);
}

TEST_CASE("WUPSButtonComboAPI_AddButtonCombo fails if invalid or empty controller mask is used") {
    WUPSButtonCombo_ComboOptions options = getDefaultComboOptions();

    WUPSButtonCombo_ComboHandle handle;
    WUPSButtonCombo_ComboStatus status;

    options.buttonComboOptions.basicCombo.controllerMask = static_cast<WUPSButtonCombo_ControllerTypes>(1 << 12);
    auto res                                             = WUPSButtonComboAPI_AddButtonCombo(&options, &handle, &status);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS);

    options.buttonComboOptions.basicCombo.controllerMask = static_cast<WUPSButtonCombo_ControllerTypes>(0);
    res                                                  = WUPSButtonComboAPI_AddButtonCombo(&options, &handle, &status);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS);
}

TEST_CASE("WUPSButtonComboAPI_AddButtonCombo fails if invalid or empty button combo is used") {
    WUPSButtonCombo_ComboOptions options = getDefaultComboOptions();

    WUPSButtonCombo_ComboHandle handle;
    WUPSButtonCombo_ComboStatus status;

    options.buttonComboOptions.basicCombo.combo = static_cast<WUPSButtonCombo_Buttons>(0);
    auto res                                    = WUPSButtonComboAPI_AddButtonCombo(&options, &handle, &status);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS);

    options.buttonComboOptions.basicCombo.combo = static_cast<WUPSButtonCombo_Buttons>(0x400000);
    res                                         = WUPSButtonComboAPI_AddButtonCombo(&options, &handle, &status);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS);
}

TEST_CASE("WUPSButtonComboAPI_AddButtonCombo fails if callback is missing") {
    WUPSButtonCombo_ComboOptions options = getDefaultComboOptions();

    WUPSButtonCombo_ComboHandle handle;
    WUPSButtonCombo_ComboStatus status;

    options.callbackOptions.callback = nullptr;
    auto res                         = WUPSButtonComboAPI_AddButtonCombo(&options, &handle, &status);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS);
}

TEST_CASE("WUPSButtonComboAPI_AddButtonCombo fails if hold duration is missing") {
    WUPSButtonCombo_ComboOptions options = getDefaultComboOptions();

    WUPSButtonCombo_ComboHandle handle;
    WUPSButtonCombo_ComboStatus status;

    options.buttonComboOptions.type               = WUPS_BUTTON_COMBO_COMBO_TYPE_HOLD;
    options.buttonComboOptions.optionalHoldForXMs = 0;
    auto res                                      = WUPSButtonComboAPI_AddButtonCombo(&options, &handle, &status);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS);
}
TEST_CASE("WUPSButtonComboAPI_AddButtonCombo fails which invalid combo type") {
    WUPSButtonCombo_ComboOptions options = getDefaultComboOptions();

    WUPSButtonCombo_ComboHandle handle;
    WUPSButtonCombo_ComboStatus status;

    options.buttonComboOptions.type = static_cast<WUPSButtonCombo_ComboType>(500);
    auto res                        = WUPSButtonComboAPI_AddButtonCombo(&options, &handle, &status);
    REQUIRE(res == WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS);
}