/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <cocos2d.h>

/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;

/**
 * `$modify` lets you extend and modify GD's classes.
 * To hook a function in Geode, simply $modify the class
 * and write a new function definition with the signature of
 * the function you want to hook.
 *
 * Here we use the overloaded `$modify` macro to set our own class name,
 * so that we can use it for button callbacks.
 *
 * Notice the header being included, you *must* include the header for
 * the class you are modifying, or you will get a compile error.
 *
 * Another way you could do this is like this:
 *
 * struct MyMenuLayer : Modify<MyMenuLayer, MenuLayer> {};
 */


class $modify(MyPlayLayer, PlayLayer) {
    void showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5) {
        PlayLayer::showNewBest(p0, p1, p2, p3, p4, p5);

        CCNode* newBestContainer = nullptr;
        auto children = this->getChildren();
        CCObject* parentObj;

        // --- Find the Container (This logic is correct) ---
        CCARRAY_FOREACH_REVERSE(children, parentObj) {
            auto potentialContainer = static_cast<CCNode*>(parentObj);
            if (potentialContainer && potentialContainer->getChildrenCount() > 0) {
                bool foundLabel = false;
                CCObject* childObj;
                CCARRAY_FOREACH(potentialContainer->getChildren(), childObj) {
                    if (dynamic_cast<CCLabelBMFont*>(childObj)) {
                        foundLabel = true;
                        break;
                    }
                }
                if (foundLabel) {
                    newBestContainer = potentialContainer;
                    break;
                }
            }
        }

        if (newBestContainer) {
            Loader::get()->queueInMainThread([newBestContainer]() {
                newBestContainer->stopAllActions();

                bool shouldHide = Mod::get()->getSettingValue<bool>("hide-popup");
                if (shouldHide) {
                    newBestContainer->setVisible(false);
                    return;
                }

                newBestContainer->setVisible(true);

                double scale = Mod::get()->getSettingValue<double>("popup-scale");
                newBestContainer->setScale(static_cast<float>(scale));

                std::string positionSetting = Mod::get()->getSettingValue<std::string>("popup-position");
                auto winSize = CCDirector::sharedDirector()->getWinSize();

                // --- POSITIONING FIX: Add a margin to prevent clipping ---
                // This value pulls the popup away from the screen edge. You can adjust it.
                float margin = 40.0f;

                // --- NEW LOGIC: Check the boolean toggle first ---
                bool useCustomPos = Mod::get()->getSettingValue<bool>("use-custom-position");

                if (useCustomPos) {
                    // If the checkbox is ticked, use the custom X and Y values
                    double customX = Mod::get()->getSettingValue<double>("custom-x-position");
                    double customY = Mod::get()->getSettingValue<double>("custom-y-position");
                    newBestContainer->setPosition(ccp(
                        static_cast<float>(customX),
                        static_cast<float>(customY)
                    ));
                }
                else {
                    // If the checkbox is not ticked, use the preset dropdown
                    std::string positionSetting = Mod::get()->getSettingValue<std::string>("popup-position");

                    if (positionSetting == "Top Left") {
                        newBestContainer->setPosition(ccp(margin, winSize.height - margin));
                        newBestContainer->setAnchorPoint(ccp(0, 1)); // Anchor by top-left corner
                    }
                    else if (positionSetting == "Top Middle") {
                        newBestContainer->setPosition(ccp(winSize.width / 2, winSize.height - margin));
                        newBestContainer->setAnchorPoint(ccp(0.5, 1)); // Anchor by top-center edge
                    }
                    else if (positionSetting == "Top Right") {
                        newBestContainer->setPosition(ccp(winSize.width - margin, winSize.height - margin));
                        newBestContainer->setAnchorPoint(ccp(1, 1)); // Anchor by top-right corner
                    }
                    else if (positionSetting == "Bottom Left") {
                        newBestContainer->setPosition(ccp(margin, margin));
                        newBestContainer->setAnchorPoint(ccp(0, 0)); // Anchor by bottom-left corner
                    }
                    else if (positionSetting == "Bottom Middle") {
                        newBestContainer->setPosition(ccp(winSize.width / 2, margin));
                        newBestContainer->setAnchorPoint(ccp(0.5, 0)); // Anchor by bottom-center edge
                    }
                    else if (positionSetting == "Bottom Right") {
                        newBestContainer->setPosition(ccp(winSize.width - margin, margin));
                        newBestContainer->setAnchorPoint(ccp(1, 0)); // Anchor by bottom-right corner
                        // If positionSetting is "Default", we do nothing and let the game handle it.
                    }
                }



                float duration = static_cast<float>(Mod::get()->getSettingValue<double>("popup-duration"));

                // Create a sequence: Delay -> Fade Out -> Remove itself
                auto sequence = CCSequence::create(
                    CCDelayTime::create(duration),      // Wait for the duration from settings
                    CCScaleTo::create(0.15f, 0.0f),
                    CCRemoveSelf::create(),             // Remove the node from the scene
                    nullptr
                );

                // Run the full animation sequence on the popup
                newBestContainer->runAction(sequence);
            });
        }
    }
};