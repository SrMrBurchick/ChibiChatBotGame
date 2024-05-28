#pragma once

#include <QObject>
#include <QColor>

/*
title 	String 	Yes 	The custom reward’s title. The title may contain a maximum of 45 characters and it must be unique amongst all of the broadcaster’s custom rewards.
cost 	Int64 	Yes 	The cost of the reward, in Channel Points. The minimum is 1 point.
prompt 	String 	No 	The prompt shown to the viewer when they redeem the reward. Specify a prompt if is_user_input_required is true. The prompt is limited to a maximum of 200 characters.
is_enabled 	Boolean 	No 	A Boolean value that determines whether the reward is enabled. Viewers see only enabled rewards. The default is true.
background_color 	String 	No 	The background color to use for the reward. Specify the color using Hex format (for example, #9147FF).
is_user_input_required 	Boolean 	No 	A Boolean value that determines whether the user needs to enter information when redeeming the reward. See the prompt field. The default is false.
is_max_per_stream_enabled 	Boolean 	No 	A Boolean value that determines whether to limit the maximum number of redemptions allowed per live stream (see the max_per_stream field). The default is false.
max_per_stream 	Integer 	No 	The maximum number of redemptions allowed per live stream. Applied only if is_max_per_stream_enabled is true. The minimum value is 1.
is_max_per_user_per_stream_enabled 	Boolean 	No 	A Boolean value that determines whether to limit the maximum number of redemptions allowed per user per stream (see the max_per_user_per_stream field). The default is false.
max_per_user_per_stream 	Integer 	No 	The maximum number of redemptions allowed per user per stream. Applied only if is_max_per_user_per_stream_enabled is true. The minimum value is 1.
is_global_cooldown_enabled 	Boolean 	No 	A Boolean value that determines whether to apply a cooldown period between redemptions (see the global_cooldown_seconds field for the duration of the cooldown period). The default is false.
global_cooldown_seconds 	Integer 	No 	The cooldown period, in seconds. Applied only if the is_global_cooldown_enabled field is true. The minimum value is 1; however, the minimum value is 60 for it to be shown in the Twitch UX.
should_redemptions_skip_request_queue 	Boolean 	No 	A Boolean value that determines whether redemptions should be set to FULFILLED status immediately when a reward is redeemed. If false, status is set to UNFULFILLED and follows the normal request queue process. The default is false.
*/
class ChannelPointsReward : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id MEMBER m_ID)
    Q_PROPERTY(QString title MEMBER Title)
    Q_PROPERTY(int cost MEMBER Cost)
    Q_PROPERTY(bool enabled MEMBER bIsEnabled)
    Q_PROPERTY(bool userInputRequired MEMBER bIsUserInputRequired NOTIFY userInputRequiredChanged)
    Q_PROPERTY(QString userInputPrompt MEMBER UserPrompt)

public:
    explicit ChannelPointsReward(QObject* Parent = nullptr);
    virtual ~ChannelPointsReward() = default;

    QString m_ID = "";
    QString Title = "";
    QString UserPrompt = "";
    int Cost = 0;
    bool bIsEnabled = false;
    bool bIsUserInputRequired = false;

signals:
    void userInputRequiredChanged(bool isRequired);
};
