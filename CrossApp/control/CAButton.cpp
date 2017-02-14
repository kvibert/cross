//
//  CAButton.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-3-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAButton.h"
#include "view/CAScale9ImageView.h"
#include "view/CAView.h"
#include "view/CAScrollView.h"
#include "dispatcher/CATouch.h"
#include "support/CAPointExtension.h"
#include "cocoa/CCSet.h"
#include "view/CALabel.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "animation/CAViewAnimation.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN

CAButton::CAButton(const CAButton::Type& buttonType)
:m_eButtonType(buttonType)
,m_eState(CAButton::State::Normal)
,m_pImageView(NULL)
,m_pLabel(NULL)
,m_sTitleFontName("")
,m_fTitleFontSize(0)
,m_bTitleBold(false)
,m_pTitleLabelSize(DSizeZero)
,m_bDefineTitleLabelSize(false)
,m_pImageSize(DSizeZero)
,m_bDefineImageSize(false)
,m_pTitleOffset(DSizeZero)
,m_bDefineTitleOffset(false)
,m_pImageOffset(DSizeZero)
,m_bDefineImageOffset(false)
,m_bTouchClick(true)
{
    m_mImageColors[CAControl::State::Normal] = CAColor_white;
    m_mImageColors[CAControl::State::Highlighted] = CAColor_white;
    m_mImageColors[CAControl::State::Disabled] = CAColor_white;
    
    m_mTitleColors[CAControl::State::Normal] = CAColor_black;
    m_mTitleColors[CAControl::State::Highlighted] = CAColor_black;
    m_mTitleColors[CAControl::State::Disabled] = CAColor_black;
    
    m_pImageView = new CAImageView();
    m_pImageView->init();
    this->insertSubview(m_pImageView, 1);
    
    m_pLabel = new CALabel();
    m_pLabel->init();
    m_pLabel->setTextAlignment(CATextAlignment::Center);
    m_pLabel->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
    m_pLabel->setNumberOfLine(1);
    this->insertSubview(m_pLabel, 1);
}

CAButton::~CAButton(void)
{
    CC_SAFE_RELEASE_NULL(m_pImageView);
    CC_SAFE_RELEASE_NULL(m_pLabel);
}

void CAButton::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}

void CAButton::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();

    this->setControlState(m_eState);
}

CAButton* CAButton::create(const CAButton::Type& buttonType)
{
    
    CAButton* btn = new CAButton(buttonType);
    
    if (btn && btn->init())
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

CAButton* CAButton::createWithFrame(const DRect& rect, const CAButton::Type& buttonType)
{
    CAButton* btn = new CAButton(buttonType);
    
    if (btn && btn->initWithFrame(rect))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

CAButton* CAButton::createWithCenter(const DRect& rect, const CAButton::Type& buttonType)
{
    CAButton* btn = new CAButton(buttonType);
    
    if (btn && btn->initWithCenter(rect))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

CAButton* CAButton::createWithLayout(const CrossApp::DLayout &layout, const CAButton::Type &buttonType)
{
    CAButton* btn = new CAButton(buttonType);
    
    if (btn && btn->initWithLayout(layout))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

bool CAButton::init()
{
    if (!CAControl::init())
    {
        return false;
    }
    
    this->setColor(CAColor_clear);
    
    switch (m_eButtonType)
    {
        case CAButton::Type::SquareRect:
            this->setBackgroundViewSquareRect();
            break;
        case CAButton::Type::RoundedRect:
            this->setBackgroundViewRoundedRect();
            break;
        default:
            break;
    }
    
    return true;
}

void CAButton::setBackgroundViewSquareRect()
{
    const CAThemeManager::stringMap& map = CAApplication::getApplication()->getThemeManager()->getThemeMap("CAButton_SquareRect");
    
    this->setBackgroundViewForState(CAControl::State::Normal,
                                    CAScale9ImageView::createWithImage(CAImage::create(map.at("backgroundView_normal"))));
    this->setBackgroundViewForState(CAControl::State::Highlighted,
                                    CAScale9ImageView::createWithImage(CAImage::create(map.at("backgroundView_highlighted"))));
    this->setBackgroundViewForState(CAControl::State::Disabled,
                                    CAScale9ImageView::createWithImage(CAImage::create(map.at("backgroundView_disabled"))));
    
    m_mTitleColors[CAControl::State::Normal] = ccc4Int(CrossApp::hex2Int(map.at("titleColor_normal")));
    m_mTitleColors[CAControl::State::Highlighted] = ccc4Int(CrossApp::hex2Int(map.at("titleColor_highlghted")));
    m_mTitleColors[CAControl::State::Disabled] = ccc4Int(CrossApp::hex2Int(map.at("titleColor_disabled")));
}

void CAButton::setBackgroundViewRoundedRect()
{
    const CAThemeManager::stringMap& map = CAApplication::getApplication()->getThemeManager()->getThemeMap("CAButton_RoundedRect");
    
    this->setBackgroundViewForState(CAControl::State::Normal,
                                    CAScale9ImageView::createWithImage(CAImage::create(map.at("backgroundView_normal"))));
    this->setBackgroundViewForState(CAControl::State::Highlighted,
                                    CAScale9ImageView::createWithImage(CAImage::create(map.at("backgroundView_highlighted"))));
    this->setBackgroundViewForState(CAControl::State::Disabled,
                                    CAScale9ImageView::createWithImage(CAImage::create(map.at("backgroundView_disabled"))));
    
    m_mTitleColors[CAControl::State::Normal] = ccc4Int(CrossApp::hex2Int(map.at("titleColor_normal")));
    m_mTitleColors[CAControl::State::Highlighted] = ccc4Int(CrossApp::hex2Int(map.at("titleColor_highlghted")));
    m_mTitleColors[CAControl::State::Disabled] = ccc4Int(CrossApp::hex2Int(map.at("titleColor_disabled")));
}

void CAButton::setBackgroundViewForState(CAControl::State state, CAView *var)
{
    CCAssert(state != CAControl::State::Selected, "CAButton does not support the use of CAControl::State::Selected");
    if (m_mBackgroundViews.contains(state))
    {
        this->removeSubview(m_mBackgroundViews.at(state));
        m_mBackgroundViews.erase(state);
    }
    
    if (var)
    {
        var->setLayout(DLayoutFill);
    }
    m_mBackgroundViews.insert(state, var);

    this->setControlState(m_eState);
    this->updateWithPreferredSize();
}

CAView* CAButton::getBackgroundViewForState(CAControl::State state)
{
    return m_mBackgroundViews.at(state);
}

void CAButton::setImageForState(CAControl::State state, CAImage* var)
{
    CCAssert(state != CAControl::State::Selected, "CAButton does not support the use of CAControl::State::Selected");
    m_mImages.erase(state);
    if (var)
    {
        m_mImages.insert(state, var);
    }

    if (m_bRunning)
    {
        this->setControlState(m_eState);
    }
}

CAImage* CAButton::getImageForState(CAControl::State state)
{
    return m_mImages.at(state);
}

void CAButton::setTitleForState(CAControl::State state, const std::string& var)
{
    CCAssert(state != CAControl::State::Selected, "CAButton does not support the use of CAControl::State::Selected");
    m_mTitles[state] = var;
    
    if (m_bRunning)
    {
        this->setControlState(m_eState);
    }
}

const std::string& CAButton::getTitleForState(CAControl::State state)
{
    return m_mTitles[state];
}

void CAButton::setImageColorForState(CAControl::State state, const CAColor4B& var)
{
    CCAssert(state != CAControl::State::Selected, "CAButton does not support the use of CAControl::State::Selected");
    m_mImageColors[state] = var;
    
    if (m_bRunning)
    {
        this->setControlState(m_eState);
    }
}

void CAButton::setTitleColorForState(CAControl::State state, const CAColor4B& var)
{
    CCAssert(state != CAControl::State::Selected, "CAButton does not support the use of CAControl::State::Selected");
    m_mTitleColors[state] = var;
    
    if (m_bRunning)
    {
        this->setControlState(m_eState);
    }
}

void CAButton::setTitleFontName(const std::string& var)
{
    if (m_sTitleFontName.compare(var))
    {
        m_sTitleFontName = var;
        m_pLabel->setFontName(m_sTitleFontName.c_str());
    }
    
    if (m_bRunning)
    {
        this->setControlState(m_eState);
    }
}

void CAButton::updateWithPreferredSize()
{
    if (m_fTitleFontSize < FLT_EPSILON)
    {
        m_pLabel->setFontSize(this->getBounds().size.height * 0.667f);
    }
}

bool CAButton::ccTouchBegan(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    if (m_eState != CAControl::State::Normal)
    {
        return false;
    }
    
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    do
    {
        CC_BREAK_IF(m_eState != CAControl::State::Normal);
        
        if (m_mFunctions.count(CAButton::Event::TouchLongPress) > 0)
        {
            CAViewAnimation::beginAnimations(m_s__StrID + "TouchLongPress", NULL);
            CAViewAnimation::setAnimationDuration(0.5f);
            CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CAButton::setTouchLongPress));
            CAViewAnimation::commitAnimations();
        }
        
        this->setControlState(CAControl::State::Highlighted);
        this->callBackFunction(CAButton::Event::TouchDown);

        return true;
    }
    while (0);
    
    return false;
}

void CAButton::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    if (m_mFunctions.count(CAButton::Event::TouchLongPress) > 0)
    {
        CAViewAnimation::removeAnimations(m_s__StrID + "TouchLongPress");
    }
    
    CC_RETURN_IF(!m_bTouchClick);
    
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (getBounds().containsPoint(point))
    {
        this->setControlState(CAControl::State::Highlighted);
        this->callBackFunction(CAButton::Event::TouchMoved);
    }
    else
    {
        this->setControlState(CAControl::State::Normal);
        this->callBackFunction(CAButton::Event::TouchMovedOutSide);
    }
}

void CAButton::ccTouchEnded(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    if (m_mFunctions.count(CAButton::Event::TouchLongPress) > 0)
    {
        CAViewAnimation::removeAnimations(m_s__StrID + "TouchLongPress");
    }
    
    CC_RETURN_IF(!m_bTouchClick);
    
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    if (m_eState == CAControl::State::Highlighted)
    {
        this->setControlState(CAControl::State::Normal);
    }
    
    if (getBounds().containsPoint(point))
    {
        this->callBackFunction(CAButton::Event::TouchUpInSide);
    }
    else
    {
        this->callBackFunction(CAButton::Event::TouchUpOutSide);
    }
}

void CAButton::ccTouchCancelled(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    if (m_mFunctions.count(CAButton::Event::TouchLongPress) > 0)
    {
        CAViewAnimation::removeAnimations(m_s__StrID + "TouchLongPress");
    }
    
    if (m_eState == CAControl::State::Highlighted)
    {
        this->setControlState(CAControl::State::Normal);
    }
    
    this->callBackFunction(CAButton::Event::TouchCancelled);
}

void CAButton::setControlState(CAControl::State var)
{
    CC_RETURN_IF(var == CAControl::State::Selected);
    m_eState = var;

    for (auto& it : m_mBackgroundViews)
    {
        this->removeSubview(it.second);
    }

    if (m_mBackgroundViews.contains(m_eState))
    {
        this->insertSubview(m_mBackgroundViews.at(m_eState), -1);
    }
    else if (m_mBackgroundViews.contains(CAControl::State::Normal))
    {
        this->insertSubview(m_mBackgroundViews.at(CAControl::State::Normal), -1);
    }

    CAImage* image = nullptr;
    std::string title = "";
    DRect imageViewCenter = DRectZero;
    DRect rect = DRectZero;
    DRect labelCenter = this->getBounds();
    float labelSize = 0;
    
    image = m_mImages.at(m_eState);
    title = m_mTitles[m_eState];

    if (image == nullptr)
    {
        image = m_mImages.at(CAControl::State::Normal);
    }
    
    if (title.empty())
    {
        title = m_mTitles[CAControl::State::Normal];
    }
    
    if (image && title.length() == 0)
    {
        DSize size = this->getBounds().size;
        DSize iSize = image->getContentSize();
        float scaleX = size.width / iSize.width * 0.75f;
        float scaleY = size.height / iSize.height * 0.75f;
        float scale = MIN(scaleX, scaleY);
        iSize = ccpMult(iSize, scale);
        
        imageViewCenter.origin = size / 2;
        imageViewCenter.size = iSize;
    }
    else if (!image && title.length() > 0)
    {
        labelSize = this->getBounds().size.height * 0.4f;
        labelCenter.origin = this->getBounds().size / 2 ;
    }
    else if (image && title.length() > 0)
    {
        DSize size = m_obContentSize;
        DSize iSize = image->getContentSize();
        float scaleX = size.width / iSize.width * 0.6f;
        float scaleY = size.height / iSize.height * 0.54f;
        float scale = MIN(scaleX, scaleY);
        iSize = ccpMult(iSize, scale);
        
        imageViewCenter.size = iSize;
        imageViewCenter.origin.x = size.width / 2;
        imageViewCenter.origin.y = size.height * 0.4f;
        
        labelSize = size.height * 0.2f;
        labelCenter.origin.x = size.width / 2;
        labelCenter.origin.y = size.height * 0.81f;
    }

    m_pImageView->setColor(m_mImageColors[m_eState]);

    if (m_bDefineImageSize)
    {
        imageViewCenter.size = m_pImageSize;
    }
    if (m_bDefineImageOffset)
    {
        imageViewCenter.origin = ccpMult(m_obContentSize, 0.5f);
        imageViewCenter.origin = ccpAdd(imageViewCenter.origin, m_pImageOffset);
    }
    m_pImageView->setCenter(imageViewCenter);
    
    
    if (image != m_pImageView->getImage())
    {
        m_pImageView->setImage(image);
    }
    m_pLabel->setColor(m_mTitleColors[m_eState]);

    
    if (m_bDefineTitleLabelSize)
    {
        labelCenter.size = m_pTitleLabelSize;
    }
    
    if(m_bDefineTitleOffset)
    {
        labelCenter.origin = ccpMult(m_obContentSize, 0.5f);
        labelCenter.origin = ccpAdd(labelCenter.origin, m_pTitleOffset);
    }
    m_pLabel->setCenter(labelCenter);
    
    if (!title.empty())
    {
        if(m_fTitleFontSize == 0)
        {
            m_fTitleFontSize = labelSize;
        }
        m_pLabel->setFontSize(m_fTitleFontSize);
    }
    
    if (strcmp(title.c_str(), m_pLabel->getText().c_str()))
    {
        m_pLabel->setText(title.c_str());
    }
}

void CAButton::interruptTouchState()
{
    CC_RETURN_IF(m_bTouchClick == false);
    m_bTouchClick = false;
    CC_RETURN_IF(m_eState != CAControl::State::Highlighted);

    this->ccTouchCancelled(NULL, NULL);
}

void CAButton::setTouchLongPress()
{
    this->callBackFunction(CAButton::Event::TouchLongPress);
}

void CAButton::setContentSize(const DSize & var)
{
    DSize size = var;
    if (m_bRecSpe && m_eButtonType != CAButton::Type::Custom)
    {
        if (m_eButtonType == CAButton::Type::SquareRect)
        {
            const CAThemeManager::stringMap& map = CAApplication::getApplication()->getThemeManager()->getThemeMap("CAButton_SquareRect");
            int h = atoi(map.at("height").c_str());
            size.height = (h == 0) ? size.height : h;
        }
        else if(m_eButtonType == CAButton::Type::RoundedRect)
        {
            const CAThemeManager::stringMap& map = CAApplication::getApplication()->getThemeManager()->getThemeMap("CAButton_RoundedRect");
            int h = atoi(map.at("height").c_str());
            size.height = (h == 0) ? size.height : h;
        }
    }
    CAView::setContentSize(size);
    
    this->updateWithPreferredSize();
    this->setControlState(m_eState);
}

void CAButton::setImageOffset(const DSize& offset)
{
    m_bDefineImageOffset = true;
    m_pImageOffset = offset;
    DRect rect = m_pImageView->getCenter();
    rect.origin = m_obContentSize/2;
    rect.origin.x += offset.width;
    rect.origin.y += offset.height;
    m_pImageView->setCenter(rect);
}

void CAButton::setImageSize(const DSize& size)
{
    m_bDefineImageSize = true;
    m_pImageSize = size;
    DRect rect = m_pImageView->getCenter();
    rect.size = m_pImageSize;
    m_pImageView->setCenter(rect);
}

void CAButton::setTitleOffset(const DSize& offset)
{
    m_bDefineTitleOffset = true;
    m_pTitleOffset = offset;
    DRect rect = m_pLabel->getCenter();
    rect.origin = m_obContentSize/2;
    rect.origin.x += offset.width;
    rect.origin.y += offset.height;
    m_pLabel->setCenter(rect);
}

void CAButton::setTitleLabelSize(const DSize& size)
{
    m_bDefineTitleLabelSize= true;
    m_pTitleLabelSize = size;
    DRect rect = m_pLabel->getCenter();
    rect.size = m_pTitleLabelSize;
    m_pLabel->setCenter(rect);
}

void CAButton::setTitleFontSize(float fontSize)
{
    m_fTitleFontSize = fontSize;
    m_pLabel->setFontSize(m_fTitleFontSize);
}

void CAButton::setTitleBold(bool bold)
{
    m_bTitleBold = bold;
    m_pLabel->setBold(bold);
}

void CAButton::setTitleTextAlignment(const CATextAlignment& var)
{
    m_pLabel->setTextAlignment(var);
}

void CAButton::addTarget(const std::function<void(CAButton*)>& function, CAButton::Event event)
{
    m_mFunctions[event] = function;
}

void CAButton::callBackFunction(CAButton::Event event)
{
    auto it = m_mFunctions.find(event);
    if (it != m_mFunctions.end())
    {
        (it->second)(this);
    }
}

NS_CC_END
