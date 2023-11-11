use bevy::{
    diagnostic::{
        DiagnosticsStore, FrameTimeDiagnosticsPlugin
    },
    prelude::*
};

use crate::components::common::ui::UISettings;

#[derive(Component)]
pub struct TextChanges;

pub fn setup_debug_info(
    mut commands: Commands,
    text_font: Res<UISettings>,
) {
    commands.spawn((
        TextBundle::from_sections([
            TextSection::from_style(TextStyle {
                font: text_font.font.clone(),
                font_size: 30.0,
                color: Color::BLACK,
            }),
        ])
        .with_style(Style {
            position_type: PositionType::Absolute,
            bottom: Val::Px(5.0),
            right: Val::Px(15.0),
            ..default()
        }),
        TextChanges,
    ));
}

pub fn debug_info_system(
    time: Res<Time>,
    diagnostics: Res<DiagnosticsStore>,
    mut query: Query<&mut Text, With<TextChanges>>,
) {
    for mut text in &mut query {
        let mut fps = 0.0;
        if let Some(fps_diagnostic) = diagnostics.get(FrameTimeDiagnosticsPlugin::FPS) {
            if let Some(fps_smoothed) = fps_diagnostic.smoothed() {
                fps = fps_smoothed;
            }
        }

        let mut frame_time = time.delta_seconds_f64();
        if let Some(frame_time_diagnostic) = diagnostics.get(FrameTimeDiagnosticsPlugin::FRAME_TIME)
        {
            if let Some(frame_time_smoothed) = frame_time_diagnostic.smoothed() {
                frame_time = frame_time_smoothed;
            }
        }

        text.sections[0].value = format!(
            "{fps:.1} fps, {frame_time:.3} ms/frame",
        );
    }
}
