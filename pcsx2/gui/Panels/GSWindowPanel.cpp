/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2010  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PrecompiledHeader.h"
#include "ConfigurationPanels.h"

using namespace pxSizerFlags;

// --------------------------------------------------------------------------------------
//  GSWindowSetting Implementation
// --------------------------------------------------------------------------------------

Panels::GSWindowSettingsPanel::GSWindowSettingsPanel( wxWindow* parent )
	: BaseApplicableConfigPanel_SpecificConfig( parent )
{
	const wxString aspect_ratio_labels[] =
	{
		_("Fit to Window/Screen"),
		_("Standard (4:3)"),
		_("Widescreen (16:9)"),
		_("Frame")
	};

	const wxString scaling_type_labels[] =
	{
		_("Fit to Window/Screen"),
		_("Integer Scaling"),
		_("Centered")
	};

	const wxString fmv_aspect_ratio_switch_labels[] =
	{
		_("Off (Default)"),
		_("Standard (4:3)"),
		_("Widescreen (16:9)")
	};

	// Warning must match the order of the VsyncMode Enum
	const wxString vsync_label[] =
	{
		_("Disabled"),
		_("Standard"),
		_("Adaptive"),
	};

	m_text_Zoom = CreateNumericalTextCtrl( this, 5 );

	m_combo_AspectRatio	= new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		ArraySize(aspect_ratio_labels), aspect_ratio_labels, wxCB_READONLY );

	m_combo_ScalingType = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		ArraySize(scaling_type_labels), scaling_type_labels, wxCB_READONLY );

	m_combo_FMVAspectRatioSwitch = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		ArraySize(fmv_aspect_ratio_switch_labels), fmv_aspect_ratio_switch_labels, wxCB_READONLY );

	m_combo_vsync = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		ArraySize(vsync_label), vsync_label, wxCB_READONLY );

	m_text_WindowWidth	= CreateNumericalTextCtrl( this, 5 );
	m_text_WindowHeight	= CreateNumericalTextCtrl( this, 5 );

	// Linux/Mac note: Exclusive Fullscreen mode is probably Microsoft specific, though
	// I'm not yet 100% certain of that.

	m_check_SizeLock	= new pxCheckBox( this, _("Disable window resize border") );
	m_check_HideMouse	= new pxCheckBox( this, _("Always hide mouse cursor") );
	m_check_ScalingCompensation = new pxCheckBox(this, _("Disable Scaling Type height compensation") );
	m_check_CloseGS		= new pxCheckBox( this, _("Hide window when paused") );
	m_check_Fullscreen	= new pxCheckBox( this, _("Default to fullscreen mode on open") );
	m_check_DclickFullscreen = new pxCheckBox( this, _("Double-click toggles fullscreen mode") );

	m_combo_AspectRatio->SetToolTip( pxEt( L"Stretch: Stretches the image to the window/screen size.\n\n"
		L"4:3: Sets the aspect ratio to 4:3, the default aspect ratio used by the PS2.\n\n"
		L"16:9: Sets the aspect ratio to widescreen 16:9, the ratio used by games when "
		L"a widescreen option or a normal widescreen patch is enabled."
		L"NOTE: Using the widescreen 16:9 aspect ratio will result in a stretched image unless "
		L"widescreen is enabled in-game or through a widescreen patch. "
		L"Widescreen is not available for all games.\n\n"
		L"Frame: Uses the aspect ratio of the frame size of the game, to keep a 1:1 pixel ratio.") );

	m_combo_ScalingType->SetToolTip( pxEt( L"Fit: Scales the image width and/or height to the window/screen size depending on the selected aspect ratio.\n\n"
		L"Integer Scaling: Scales the image to the highest integer magnification of the selected aspect ratio that "
		L"fits within the window/screen.\n\n"
		L"Centered: Displays the image at the native size used by the game, as long as it fits within the window/screen.") );

	m_combo_FMVAspectRatioSwitch->SetToolTip( pxEt( L"Off: Disables temporary aspect ratio switch.\n\n"
		L"4:3: Temporarily switch to a 4:3 aspect ratio while an FMV plays to correctly display an 4:3 FMV.\n\n"
		L"16:9: Temporarily switch to a 16:9 aspect ratio while an FMV plays to correctly display a widescreen 16:9 FMV.") );

	m_text_Zoom->SetToolTip( pxEt( L"Zoom = 100: Fit the entire image to the window without any cropping.\nAbove/Below 100: Zoom In/Out\n0: Automatic-Zoom-In untill the black-bars are gone (Aspect ratio is kept, some of the image goes out of screen).\n  NOTE: Some games draw their own black-bars, which will not be removed with '0'.\n\nKeyboard: CTRL + NUMPAD-PLUS: Zoom-In, CTRL + NUMPAD-MINUS: Zoom-Out, CTRL + NUMPAD-*: Toggle 100/0"
	) );

	m_combo_vsync->SetToolTip( pxEt( L"Vsync eliminates screen tearing but typically has a big performance hit. It usually only applies to fullscreen mode, and may not work with all GS plugins."
	) );

	m_check_HideMouse->SetToolTip( pxEt( L"Check this to force the mouse cursor invisible inside the GS window; useful if using the mouse as a primary control device for gaming.  By default the mouse auto-hides after 2 seconds of inactivity."
	) );

	m_check_ScalingCompensation->SetToolTip( pxEt( L"Don't let Scaling Types compensate(double) the height for games that put out an image that is half as high as displayed. This will half the height for some games. Only for purists.\n\n  NOTE: Only available when \"Frame\" Aspect Ratio or \"Integer\" or \"Centered\" Scaling Type is selected."
	) );

	m_check_Fullscreen->SetToolTip( pxEt( L"Enables automatic mode switch to fullscreen when starting or resuming emulation. You can still toggle fullscreen display at any time using alt-enter."
	) );

	m_check_CloseGS->SetToolTip( pxEt( L"Completely closes the often large and bulky GS window when pressing ESC or pausing the emulator."
	) );

	// ----------------------------------------------------------------------------
	//  Layout and Positioning

	wxBoxSizer& s_customsize( *new wxBoxSizer( wxHORIZONTAL ) );
	s_customsize += m_text_WindowWidth;
	s_customsize += Label( L"x" ) | StdExpand();
	s_customsize += m_text_WindowHeight;

	wxFlexGridSizer& s_AspectRatio( *new wxFlexGridSizer( 2, StdPadding, StdPadding ) );
	//s_AspectRatio.AddGrowableCol( 0 );
	s_AspectRatio.AddGrowableCol( 1 );

	s_AspectRatio += Label(_("Aspect Ratio:")) | pxMiddle;
	s_AspectRatio += m_combo_AspectRatio | pxAlignRight;
	s_AspectRatio += Label(_("Scaling Type:")) | pxMiddle;
	s_AspectRatio += m_combo_ScalingType | pxAlignRight;
	s_AspectRatio += Label(_("FMV Aspect Ratio Override:")) | pxMiddle;
	s_AspectRatio += m_combo_FMVAspectRatioSwitch | pxAlignRight;
	s_AspectRatio += Label(_("Custom Window Size:")) | pxMiddle;
	s_AspectRatio += s_customsize | pxAlignRight;

	s_AspectRatio += Label(_("Zoom:")) | pxMiddle;
	s_AspectRatio += m_text_Zoom | pxAlignRight;

	wxFlexGridSizer& s_vsync( *new wxFlexGridSizer( 2, StdPadding, StdPadding ) );
	s_vsync.AddGrowableCol( 1 );

	s_vsync += Label(_("Wait for Vsync on refresh:")) | pxMiddle;
	s_vsync += m_combo_vsync | pxAlignRight;

	*this += s_AspectRatio | StdExpand();
	*this += new wxStaticLine( this ) | StdExpand();

	*this += m_check_SizeLock;
	*this += m_check_HideMouse;
	*this += m_check_CloseGS;
	*this += m_check_ScalingCompensation;
	*this += new wxStaticLine( this ) | StdExpand();

	*this += m_check_Fullscreen;
	*this += m_check_DclickFullscreen;
	*this += new wxStaticLine( this ) | StdExpand();

	*this += s_vsync | StdExpand();

	wxBoxSizer* centerSizer = new wxBoxSizer( wxVERTICAL );
	*centerSizer += GetSizer()	| pxCenter;
	SetSizer( centerSizer, false );

	Bind(wxEVT_COMBOBOX, &Panels::GSWindowSettingsPanel::ScalingTypeChanged, this);

	AppStatusEvent_OnSettingsApplied();
}

void Panels::GSWindowSettingsPanel::AppStatusEvent_OnSettingsApplied()
{
	ApplyConfigToGui( *g_Conf );
}

void Panels::GSWindowSettingsPanel::ApplyConfigToGui( AppConfig& configToApply, int flags )
{
	const AppConfig::GSWindowOptions& conf( configToApply.GSWindow );

	if( !(flags & AppConfig::APPLY_FLAG_FROM_PRESET) )	
	{//Presets don't control these values
		m_check_CloseGS		->SetValue( conf.CloseOnEsc );
		m_check_Fullscreen	->SetValue( conf.DefaultToFullscreen );
		m_check_HideMouse	->SetValue( conf.AlwaysHideMouse );
		m_check_ScalingCompensation->SetValue( conf.DisableScalingCompensation );
		m_check_ScalingCompensation->Enable( ( conf.AspectRatio != AspectRatio_Stretch && conf.ScalingType != ScalingType_Fit ) || conf.AspectRatio == AspectRatio_Frame );
		m_check_SizeLock	->SetValue( conf.DisableResizeBorders );

		m_combo_AspectRatio	->SetSelection( enum_cast( conf.AspectRatio ) );
		m_combo_ScalingType	->SetSelection( enum_cast( conf.ScalingType ) );
		m_combo_ScalingType	->Enable( conf.AspectRatio != AspectRatio_Stretch );
		m_combo_FMVAspectRatioSwitch->SetSelection( enum_cast( conf.FMVAspectRatioSwitch ) );
		m_text_Zoom			->ChangeValue( conf.Zoom.ToString() );

		m_check_DclickFullscreen ->SetValue( conf.IsToggleFullscreenOnDoubleClick );

		m_text_WindowWidth	->ChangeValue( wxsFormat( L"%d", conf.WindowSize.GetWidth() ) );
		m_text_WindowHeight	->ChangeValue( wxsFormat( L"%d", conf.WindowSize.GetHeight() ) );
	}

	m_combo_vsync->SetSelection(enum_cast(configToApply.EmuOptions.GS.VsyncEnable));
	m_combo_vsync->Enable( !configToApply.EnablePresets );//grayed-out when presets are enabled
}

void Panels::GSWindowSettingsPanel::ScalingTypeChanged(wxCommandEvent &event)
{
	m_combo_ScalingType->Enable(m_combo_AspectRatio->GetSelection() != 0);
	m_check_ScalingCompensation->Enable((m_combo_AspectRatio->GetSelection() != 0 && m_combo_ScalingType->GetSelection() != 0) || m_combo_AspectRatio->GetSelection() == 3);

	event.Skip();
}

void Panels::GSWindowSettingsPanel::Apply()
{
	AppConfig::GSWindowOptions& appconf( g_Conf->GSWindow );
	Pcsx2Config::GSOptions& gsconf( g_Conf->EmuOptions.GS );

	appconf.CloseOnEsc				= m_check_CloseGS	->GetValue();
	appconf.DisableScalingCompensation = m_check_ScalingCompensation->GetValue();
	appconf.DefaultToFullscreen		= m_check_Fullscreen->GetValue();
	appconf.AlwaysHideMouse			= m_check_HideMouse	->GetValue();
	appconf.DisableResizeBorders	= m_check_SizeLock	->GetValue();

	appconf.AspectRatio		= (AspectRatioType)m_combo_AspectRatio->GetSelection();
	appconf.ScalingType		= (ScalingTypes)m_combo_ScalingType->GetSelection();
	appconf.FMVAspectRatioSwitch = (FMVAspectRatioSwitchType)m_combo_FMVAspectRatioSwitch->GetSelection();
	appconf.Zoom			= Fixed100::FromString( m_text_Zoom->GetValue() );

	gsconf.VsyncEnable = static_cast<VsyncMode>(m_combo_vsync->GetSelection());

	appconf.IsToggleFullscreenOnDoubleClick = m_check_DclickFullscreen->GetValue();

	long xr, yr = 1;

	if( !m_text_WindowWidth->GetValue().ToLong( &xr ) || !m_text_WindowHeight->GetValue().ToLong( &yr ) )
		throw Exception::CannotApplySettings( this )
			.SetDiagMsg(L"User submitted non-numeric window size parameters!")
			.SetUserMsg(_("Invalid window dimensions specified: Size cannot contain non-numeric digits! >_<"));

	appconf.WindowSize.x	= xr;
	appconf.WindowSize.y	= yr;
}
