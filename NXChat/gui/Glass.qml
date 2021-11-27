import QtQuick 2.12
import "Base"
QtObject {
    function hsluv(h, s, l, a) { return utils.hsluv(h, s, l, a) }
    function hsl(h, s, l)      { return utils.hsl(h, s, l) }
    function hsla(h, s, l, a)  { return utils.hsla(h, s, l, a) }
    id: theme
    property real uiScale: 1.0
    property int minimumSupportedWidth:  240 * uiScale
    property int minimumSupportedHeight: 120 * uiScale
    property int contentIsWideAbove:     472 * uiScale
    property int baseElementsHeight:       48 * uiScale
    property int spacing:                  12 * uiScale
    property int radius:                   4 * uiScale
    property int animationDuration:        100
    property real loadingElementsOpacity:  0.8
    property real disabledElementsOpacity: 0.3
    readonly property QtObject fontSize: QtObject
    {
        property int smaller: 13 * uiScale
        property int small:   13 * uiScale
        property int normal:  16 * uiScale
        property int big:     20 * uiScale
        property int bigger:  32 * uiScale
        property int biggest: 48 * uiScale
    }
    readonly property QtObject fontFamily: QtObject
    {
        property string sans: "Roboto"
        property string mono: "Hack"
    }
    readonly property QtObject colors: QtObject
    {
        property int hue: 240
        property real intensity:               1.0
        property real coloredTextIntensity:    intensity * 71
        property real dimColoredTextIntensity: intensity * 60
        property int saturation:               60
        property int bgSaturation:             saturation / 1.5
        property int coloredTextSaturation:    saturation + 20
        property int dimColoredTextSaturation: saturation
        property real opacity: 0.7
        property color weakBackground:   hsluv(hue, bgSaturation, intensity * 2.5, opacity)
        property color mediumBackground: hsluv(hue, bgSaturation, intensity * 7, opacity)
        property color strongBackground: hsluv(hue, bgSaturation * 2, intensity, opacity)
        property color accentBackground:    hsluv(hue, saturation, intensity * 42, 1)
        property color accentElement:       hsluv(hue, saturation * 1.5, intensity * 52, 1)
        property color strongAccentElement: hsluv(hue, saturation * 1.5, intensity * 72, 1)
        property color positiveBackground:
            hsluv(155, saturation * 1.5, intensity * 65, 1)
        property color middleBackground:
            hsluv(60, saturation * 1.5, intensity * 65, 1)
        property color negativeBackground:
            hsluv(0, saturation * 1.5, intensity * 54, 1)
        property color alertBackground: negativeBackground
        property color brightText:  hsluv(0, 0, intensity * 100)
        property color text:        hsluv(0, 0, intensity * 85)
        property color halfDimText: hsluv(0, 0, intensity * 72)
        property color dimText:     hsluv(0, 0, intensity * 60)
        property color positiveText: hsluv(155, coloredTextSaturation, coloredTextIntensity)
        property color warningText:  hsluv(60, coloredTextSaturation, coloredTextIntensity)
        property color errorText:    hsluv(0, coloredTextSaturation, coloredTextIntensity)
        property color accentText:   hsluv(hue, coloredTextSaturation, coloredTextIntensity)
        property color link: hsluv(hue, coloredTextSaturation, coloredTextIntensity)
        property color code: hsluv(hue + 10, coloredTextSaturation, coloredTextIntensity)
        NumberAnimation on hue
        {
            running:  false
            from:     0
            to:       360
            duration: 10000
            loops:    Animation.Infinite
        }
    }
    readonly property QtObject icons: QtObject
    {
        property string preferredPack: "thin"
        property color colorize:         hsluv(0, 0, colors.intensity * 90)
        property color disabledColorize: "white"
        property int smallDimension: 16 * uiScale
        property int dimension: 22 * uiScale
    }
    readonly property QtObject controls: QtObject
    {
        readonly property QtObject scrollBar: QtObject
        {
            property int width: theme.spacing
            property color track: colors.strongBackground
            property color slider:        colors.accentElement
            property color hoveredSlider: colors.accentElement
            property color pressedSlider: colors.strongAccentElement
            property int sliderPadding: 2
            property int sliderRadius:  theme.radius
        }
        readonly property QtObject box: QtObject
        {
            property int defaultWidth: minimumSupportedWidth
            property color background: colors.mediumBackground
            property int radius:       theme.radius
        }
        readonly property QtObject popup: QtObject
        {
            property int defaultWidth:    minimumSupportedWidth * 1.75
            property color background:    colors.mediumBackground
            property color windowOverlay: hsluv(0, 0, 0, 0.7)
        }
        readonly property QtObject header: QtObject
        {
            property color background: colors.mediumBackground
        }
        readonly property QtObject button: QtObject
        {
            property color background:       colors.strongBackground
            property color text:             colors.text
            property color focusedBorder:    colors.accentElement
            property int focusedBorderWidth: 2
            property color hoveredOverlay: hsluv(0, 0, 50, 0.2)
            property color pressedOverlay: hsluv(0, 0, 50, 0.5)
            property color checkedOverlay: colors.accentBackground
        }
        readonly property QtObject tab: QtObject
        {
            property color text:                controls.button.text
            property color background:          controls.button.background
            property color alternateBackground: hsluv(
                colors.hue,
                colors.bgSaturation * 1.25,
                colors.intensity * 4,
                Math.max(0.6, colors.opacity)
            )
            property color bottomLine:       background
            property color focusedBorder:    colors.accentElement
            property int focusedBorderWidth: 1
            property color hoveredOverlay: controls.button.hoveredOverlay
            property color pressedOverlay: controls.button.pressedOverlay
            property color checkedOverlay: controls.button.checkedOverlay
        }
        readonly property QtObject menu: QtObject
        {
            property color background: hsluv(
                colors.hue,
                colors.bgSaturation * 2,
                colors.intensity,
                Math.max(0.9, colors.opacity),
            )
            property color border:     "black"
            property real borderWidth: 2
        }
        readonly property QtObject menuItem: QtObject
        {
            property color background: "transparent"
            property color text:       controls.button.text
            property color hoveredOverlay: controls.button.hoveredOverlay
            property color pressedOverlay: controls.button.hoveredOverlay
            property color checkedOverlay: controls.button.hoveredOverlay
        }
        readonly property QtObject checkBox: QtObject
        {
            property color checkIconColorize: colors.accentElement
            property color boxBackground:     controls.button.background
            property int boxSize:             24 * uiScale
            property color boxBorder:        "black"
            property color boxHoveredBorder: colors.accentElement
            property color boxPressedBorder: colors.strongAccentElement
            property color text: controls.button.text
            property color subtitle: colors.dimText
        }
        readonly property QtObject listView: QtObject
        {
            property color highlight: hsluv(
                colors.hue, colors.bgSaturation * 2, 0, colors.opacity / 2,
            )
            property color highlightBorder: colors.strongAccentElement
            property int highlightBorderThickness: 1
        }
        readonly property QtObject textField: QtObject
        {
            property color background:        colors.strongBackground
            property color focusedBackground: background
            property int borderWidth:     1
            property color border:        "transparent"
            property color focusedBorder: colors.accentElement
            property color errorBorder:   colors.negativeBackground
            property color text:            colors.text
            property color focusedText:     colors.text
            property color placeholderText: colors.dimText
        }
        readonly property QtObject textArea: QtObject
        {
            property color background: colors.strongBackground
            property int borderWidth:     1
            property color border:        "transparent"
            property color focusedBorder: colors.accentElement
            property color errorBorder:   colors.negativeBackground
            property color text:            colors.text
            property color placeholderText: controls.textField.placeholderText
        }
        readonly property QtObject toolTip: QtObject
        {
            property color background: colors.strongBackground
            property color text:       colors.text
            property color border:     "black"
            property int borderWidth:  2
        }
        readonly property QtObject progressBar: QtObject
        {
            property int height:             Math.max(2, spacing / 2)
            property color background:       colors.strongBackground
            property color foreground:       colors.accentElement
            property color pausedForeground: colors.middleBackground
            property color errorForeground:  colors.negativeBackground
        }
        readonly property QtObject circleProgressBar: QtObject
        {
            property int thickness:          Math.max(2, spacing / 2)
            property color background:       colors.strongBackground
            property color foreground:       colors.accentElement
            property color errorForeground:  colors.negativeBackground
            property color text:             colors.text
            property real indeterminateSpan: 0.5  // 0-1
        }
        readonly property QtObject slider: QtObject
        {
            property int radius:       2
            property int height:       controls.progressBar.height
            property color background: controls.progressBar.background
            property color foreground: controls.progressBar.foreground
            readonly property QtObject handle: QtObject
            {
                property int size: 20
                property color inside: hsluv(0, 0, 90)
                property color pressedInside: "white"
                property color border: "black"
                property color pressedBorder: colors.strongAccentElement
            }
        }
        readonly property QtObject avatar: QtObject
        {
            property int size:        baseElementsHeight
            property int compactSize: baseElementsHeight / 2
            property int radius:      theme.radius
            readonly property QtObject hoveredImage: QtObject
            {
                property int size: 192
                property color background: hsluv(0, 0, 0, 0.4)
            }
            readonly property QtObject background: QtObject
            {
                property int saturation: colors.saturation
                property int lightness:  Math.min(50, colors.intensity * 23)
                property real opacity:   1.0
            }
            readonly property QtObject letter: QtObject
            {
                property int saturation: colors.saturation + 20
                property int lightness:  colors.intensity * 60
                property real opacity:   1.0
            }
        }
        readonly property QtObject displayName: QtObject
        {
            property int saturation:    colors.coloredTextSaturation
            property int lightness:     colors.coloredTextIntensity
            property int dimSaturation: colors.dimColoredTextSaturation
            property int dimLightness:  colors.dimColoredTextIntensity
        }
        readonly property QtObject presence: QtObject
        {
            property color online:      colors.positiveBackground
            property color unavailable: colors.middleBackground
            property color offline:     hsluv(0, 0, 60, 1)
            property color border:      "black"
            property int borderWidth:   2 * uiScale
            property real opacity:      1.0
            property real radius:       6.0 * uiScale
        }
    }
    readonly property QtObject ui: QtObject
    {
        property url image: ""
        property point gradientStart: Qt.point(0, 0)
        property point gradientEnd:   Qt.point(window.width, window.height)
        property color gradientStartColor: hsluv(0, 0, 0, 0.5)
        property color gradientEndColor: hsluv(0, 0, 0, 0.5)
    }
    readonly property QtObject mainPane: QtObject
    {
        property color background: "transparent"
        readonly property QtObject topBar: QtObject
        {
            property color background:       colors.strongBackground
            property color nameVersionLabel: colors.text
        }
        readonly property QtObject accountBar: QtObject
        {
            property color background: colors.mediumBackground
            readonly property QtObject account: QtObject
            {
                property real collapsedOpacity: 0.3
                property color background:      "transparent"
                property int avatarRadius:      controls.avatar.radius
                property color selectedBackground:       colors.accentBackground
                property real selectedBackgroundOpacity: 0.3
                property color selectedBorder:           colors.strongAccentElement
                property int selectedBorderSize:         1
                readonly property QtObject unreadIndicator: QtObject
                {
                    property color background: colors.strongBackground
                    property color text:       colors.accentText
                    property bool bold:        false
                    property color border:     Qt.darker(text, 2)
                    property int borderWidth:  1
                    property int radius:       theme.radius / 2
                    property color highlightBackground: colors.strongBackground
                    property color highlightText:       colors.errorText
                    property bool highlightBold:        false
                    property color highlightBorder:     Qt.darker(highlightText, 2)
                    property int highlightBorderWidth:  1
                    property int highlightRadius:       theme.radius / 2
                }
            }
        }
        readonly property QtObject listView: QtObject
        {
            property color background:    colors.mediumBackground
            property real offlineOpacity: 0.5
            readonly property QtObject account: QtObject
            {
                property real collapsedOpacity:     0.3
                property color background:          "transparent"
                property color name:                colors.text
                property int avatarRadius:          controls.avatar.radius
                property int collapsedAvatarRadius: controls.avatar.size / 2
            }
            readonly property QtObject room: QtObject
            {
                property real leftRoomOpacity: 0.65
                property color background:    "transparent"
                property color name:          colors.text
                property color unreadName:    colors.brightText
                property color lastEventDate: colors.halfDimText
                property color subtitle:      colors.dimText
                property color subtitleQuote: chat.message.quote
                property int avatarRadius:          controls.avatar.radius
                property int collapsedAvatarRadius: controls.avatar.radius
                readonly property QtObject unreadIndicator: QtObject
                {
                    property color background: colors.strongBackground
                    property color text:       colors.accentText
                    property bool bold:        false
                    property color border:     Qt.darker(text, 2)
                    property int borderWidth:  1
                    property int radius:       theme.radius / 2
                    property color highlightBackground: colors.strongBackground
                    property color highlightText:       colors.errorText
                    property bool highlightBold:        false
                    property color highlightBorder:     Qt.darker(highlightText, 2)
                    property int highlightBorderWidth:  1
                    property int highlightRadius:       theme.radius / 2
                }
            }
        }
        readonly property QtObject bottomBar: QtObject
        {
            property color background:               "transparent"
            property color settingsButtonBackground: colors.strongBackground
            property color filterFieldBackground:    colors.strongBackground
        }
    }
    readonly property QtObject chat: QtObject
    {
        readonly property QtObject roomHeader: QtObject
        {
            property color background: colors.strongBackground
            property color name:       colors.text
            property color topic:      colors.dimText
        }
        readonly property QtObject roomPane: QtObject
        {
            property color background: "transparent"
            readonly property QtObject topBar: QtObject
            {
                property color background: colors.strongBackground
            }
            readonly property QtObject listView: QtObject
            {
                property color background: colors.mediumBackground
                readonly property QtObject member: QtObject
                {
                    property real invitedOpacity: 0.5
                    property color background: "transparent"
                    property color name:       colors.text
                    property color subtitle:   colors.dimText
                    property color adminIcon:     hsluv(60, colors.saturation * 2.25, 60)
                    property color moderatorIcon: adminIcon
                    property color invitedIcon:   hsluv(0, colors.saturation * 2.25, 60)
                }
            }
            readonly property QtObject roomSettings: QtObject
            {
                property color background: colors.mediumBackground
            }
            readonly property QtObject bottomBar: QtObject
            {
                property color background: colors.strongBackground
                readonly property QtObject inviteButton: QtObject
                {
                    property color background: "transparent"
                }
                readonly property QtObject filterMembers: QtObject
                {
                    property color background: "transparent"
                }
            }
        }
        readonly property QtObject eventList: QtObject
        {
            property color background: "transparent"
        }
        readonly property QtObject message: QtObject
        {
            property int avatarSize:          56 * uiScale
            property int collapsedAvatarSize: 32 * uiScale
            property int avatarRadius:        controls.avatar.radius
            property int radius:            theme.radius
            property int horizontalSpacing: theme.spacing / 1.25
            property int verticalSpacing:   theme.spacing / 1.75
            property color focusedHighlight:       colors.accentBackground
            property real focusedHighlightOpacity: 0.4
            property color background:        colors.weakBackground
            property color ownBackground:     colors.mediumBackground
            property color checkedBackground: colors.accentBackground
            property color body:        colors.text
            property color date:        colors.dimText
            property color localEcho:   colors.dimText
            property color readCounter: colors.accentText
            property color redactedBody: colors.dimText
            property color noticeBody:    colors.halfDimText
            property int noticeLineWidth: 1 * uiScale
            property color quote: hsluv(
                135, colors.coloredTextSaturation, colors.coloredTextIntensity,
            )
            property color link:  colors.link
            property color code:  colors.code
            property string styleSheet:
                "* { white-space: pre-wrap }" +
                "a { color: " + link  + " }" +
                "p { margin-top: 0 }" +
                "code { font-family: " + fontFamily.mono + "; " +
                       "color: "       + code            + " }" +
                "h1, h2, h3 { font-weight: normal }" +
                "h1 { font-size: " + fontSize.biggest + "px }" +
                "h2 { font-size: " + fontSize.bigger + "px }" +
                "h3 { font-size: " + fontSize.big + "px }" +
                "h4 { font-size: " + fontSize.normal + "px }" +
                "h5 { font-size: " + fontSize.small + "px }" +
                "h6 { font-size: " + fontSize.smaller + "px }" +
                "table { margin-top: " + theme.spacing + "px; " +
                "        margin-bottom: " + theme.spacing + "px }" +
                "td { padding-left: " + theme.spacing / 2 + "px; " +
                "     padding-right: " + theme.spacing / 2 + "px; " +
                "     padding-top: " + theme.spacing / 4 + "px; " +
                "     padding-bottom: " + theme.spacing / 4 + "px } " +
                "li { margin-top: " + theme.spacing + "px; " +
                "     margin-bottom: " + theme.spacing + "px } " +
                ".sender { margin-bottom: " + spacing / 2 + " }" +
                ".quote  { color: " + quote + " }" +
                ".mention { text-decoration: none; }" +
                ".room-id-mention, .room-alias-mention { font-weight: bold; }"
            property string styleInclude:
                '<style type"text/css">\n' + styleSheet + '\n</style>\n'
            property real thumbnailCheckedOverlayOpacity: 0.4
        }
        readonly property QtObject daybreak: QtObject
        {
            property color background: colors.mediumBackground
            property color text:       colors.text
            property int radius:       theme.radius
        }
        readonly property QtObject inviteBanner: QtObject
        {
            property color background: colors.mediumBackground
        }
        readonly property QtObject leftBanner: QtObject
        {
            property color background: colors.mediumBackground
        }
        readonly property QtObject unknownDevices: QtObject
        {
            property color background: colors.mediumBackground
        }
        readonly property QtObject typingMembers: QtObject
        {
            property color background: hsluv(
                colors.hue, colors.saturation, colors.intensity * 9, 0.52
            )
        }
        readonly property QtObject replyBar: QtObject
        {
            property color background: chat.typingMembers.background
        }
        readonly property QtObject fileTransfer: QtObject
        {
            property color background: chat.typingMembers.background
        }
        readonly property QtObject userAutoCompletion: QtObject
        {
            property color background:   chat.typingMembers.background
            property int avatarsRadius:  controls.avatar.radius
            property color displayNames: colors.text
            property color userIds:      colors.dimText
        }
        readonly property QtObject composer: QtObject
        {
            property color background: colors.strongBackground
            readonly property QtObject uploadButton: QtObject
            {
                property color background: "transparent"
            }
        }
    }
    readonly property QtObject mediaPlayer: QtObject
    {
        readonly property QtObject hoverPreview: QtObject
        {
            property int maxHeight: 192
        }
        readonly property QtObject progress: QtObject
        {
            property int height:        8
            property color background:  hsluv(0, 0, 0, 0.5)
        }
        readonly property QtObject controls: QtObject
        {
            property int iconSize:          icons.dimension
            property int volumeSliderWidth: 100
            property int speedSliderWidth:  100
            property color background:      hsluv(
                colors.hue, colors.saturation * 1.25, colors.intensity * 2, 0.85,
            )
        }
    }
}
