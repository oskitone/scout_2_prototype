include <../../parts_cafe/openscad/battery_holder.scad>;
include <../../parts_cafe/openscad/battery_holder_fixtures.scad>;
include <../../parts_cafe/openscad/support_bar.scad>;
include <../../parts_cafe/openscad/keys.scad>;
include <../../parts_cafe/openscad/pcb_base.scad>;
include <../../parts_cafe/openscad/pcb.scad>;
include <../../parts_cafe/openscad/speaker-AZ40R.scad>;

module scout_2_prototype(
    kicad_edge_cuts = EXAMPLE_EDGE_CUTS,
    kicad_screw_positions = [[0,0]],
    kicad_wire_relief_positions = [[0,0]],

    fillet_keys = true,
    keys_length = SCOUT_NATURAL_KEY_LENGTH,

    pcb_silkscreen_path = "../kicad/XYZ-brd.svg",

    padding = 4,
    tolerance = .1,

    battery_holder_web_length = BATTERY_HOLDER_FIXTURE_HITCH_WEB_LENGTH / 2,

    show_base = true,
    show_battery_holder = true,
    show_speaker = true,
    show_pcb = true,
    show_pcb_silkscreen = true,
    show_mounting_rail = true,
    show_keys = true,

    quick_preview = $preview
) {
    e = .041;

    keys_total_width = get_keys_total_width(25);
    keys_total_length = get_mounted_keys_total_length(tolerance);

    base_width = keys_total_width + padding * 2;

    pcb_dimensions = get_pcb_dimensions(kicad_edge_cuts);
    pcb_component_offset_position = get_pcb_component_offset_position(kicad_edge_cuts);
    screw_positions = get_pcb_component_positions(
        kicad_screw_positions, pcb_component_offset_position, pcb_dimensions);
    wire_relief_positions = get_pcb_component_positions(
        kicad_wire_relief_positions, pcb_component_offset_position, pcb_dimensions);

    battery_holder_dimensions = get_battery_holder_dimensions(
        count = 3,
        tolerance = tolerance
    );

    min_pcb_bottom_clearance = battery_holder_dimensions.z + MIN_PCB_BOTTOM_CLEARANCE;

    pcb_position = [
        (base_width - pcb_dimensions.x) / 2,
        keys_total_length
            - screw_positions[0].y - KEYS_MOUNT_LENGTH / 2
            + padding,
        get_pcb_base_total_height(
            min_clearance = min_pcb_bottom_clearance,
            pcb_mount_post_ceiling = PCB_MOUNT_POST_CEILING
        )
    ];

    base_length = pcb_position.y + pcb_dimensions.y + padding;

    keys_position = [
        (base_width - keys_total_width) / 2,
        pcb_position.y
            + screw_positions[0].y + KEYS_MOUNT_LENGTH / 2
            - keys_total_length,
        pcb_position.z + PCB_HEIGHT + SPST_ACTUATOR_HEIGHT_OFF_PCB
    ];

    // TODO: obviate battery_holder_position amendments here
    battery_holder_position = [
        (BATTERY_HOLDER_DEFAULT_WALL + tolerance) -(battery_holder_dimensions.x / 2)
            + pcb_position.x + screw_positions[0].x,
        (BATTERY_HOLDER_DEFAULT_WALL + tolerance)
            + (BATTERY_HOLDER_FIXTURE_HITCH_LENGTH + battery_holder_web_length)
            + padding,
        PCB_BASE_BASE_HEIGHT
    ];

    echo(str("battery_holder_dimensions = ", battery_holder_dimensions));

    speaker_fixture_diameter = get_speaker_fixture_diameter(tolerance);
    speaker_position = [
        pcb_position.x + pcb_dimensions.x * (2/3),
        speaker_fixture_diameter / 2 + padding,
        PCB_BASE_BASE_HEIGHT - e
    ];

    module _base(
        support_bar_pcb_xs = [0, SPST_PLOT * 14, SPST_PLOT * 22],
        support_bar_pcb_y = 14,
        support_bar_widths = [SPST_PLOT * 6, SPST_PLOT * 3, SPST_PLOT * 7]
    ) {
        z = PCB_BASE_BASE_HEIGHT - e;

        pcb_base(
            width = base_width,
            length = base_length,
            min_pcb_bottom_clearance = min_pcb_bottom_clearance,
            screw_positions = [ for (xy = screw_positions) [
                xy.x + pcb_position.x,
                xy.y + pcb_position.y
            ] ],
            tolerance = tolerance,
            show_dfm = !$preview
        );

        translate([
            -(BATTERY_HOLDER_DEFAULT_WALL + tolerance),
            -(BATTERY_HOLDER_DEFAULT_WALL + tolerance),
            0
        ]) {
            battery_holder_hitches(
                tolerance = tolerance,
                battery_holder_position = battery_holder_position,
                battery_holder_dimensions = battery_holder_dimensions,
                web_length = battery_holder_web_length
            );
        }

        translate(speaker_position) {
            speaker_fixture(
                tab_cavity_count = 2,
                tab_cavity_rotation = 90,
                tolerance = tolerance,
                quick_preview = quick_preview
            );
        }

        for (i = [0 : len(support_bar_pcb_xs) - 1]) {
            x = support_bar_pcb_xs[i];
            width = support_bar_widths[i];
            height = pcb_position.z - PCB_BASE_BASE_HEIGHT;

            translate([
                pcb_position.x + x,
                pcb_position.y + support_bar_pcb_y - SUPPORT_BAR_LENGTH / 2,
                PCB_BASE_BASE_HEIGHT - e
            ]) {
                render() support_bar(
                    width = width,
                    height = height + e,
                    web_chamfer = height / 2
                );
            }
        }
    }

    module _keys(mounting_rail = false) {
        keys(
            count = 25,
            natural_length = keys_length,
            mount_height = mounting_rail
                ? SPST_ACTUATOR_HEIGHT_OFF_PCB
                : SCOUT_NATURAL_KEY_HEIGHT,
            mount_hole_xs = [ for (xy = screw_positions)
                xy.x + keys_position.x - KEY_GUTTER + tolerance
            ],
            front_fillet = fillet_keys ? KEY_FRONT_FILLET : 0,
            sides_fillet = fillet_keys ? KEY_SIDES_FILLET : 0,
            tolerance = tolerance,
            include_mount = true,
            include_natural = !mounting_rail,
            include_accidental = !mounting_rail,
            include_cantilevers = !mounting_rail,
            quick_preview = quick_preview
        );
    }

    if (show_base) {
        _base();
    }

    if (show_battery_holder) {
        translate([0,0,BATTERY_HOLDER_DEFAULT_FLOOR -e]) translate(battery_holder_position) {
            battery_holder(
                tolerance = tolerance,
                quick_preview = quick_preview
            );
        }
    }

    if (show_speaker) {
        translate(speaker_position) {
            % speaker();
        }
    }

    if (show_pcb) {
        translate(pcb_position) {
            pcb(
                width = pcb_dimensions.x,
                length = pcb_dimensions.y,
                hole_positions =
                    cat(wire_relief_positions, screw_positions),
                show_silkscreen = show_pcb_silkscreen,
                silkscreen_path = pcb_silkscreen_path
            );
        }
    }

    if (show_mounting_rail) {
        translate([
            keys_position.x,
            keys_position.y,
            pcb_position.z + PCB_HEIGHT
        ]) _keys(mounting_rail = true);
    }

    if (show_keys) {
        translate(keys_position) _keys();
    }

}

scout_2_prototype(
    kicad_edge_cuts = [
        [[58.628586, 62.273], [157.861, 62.273]],
        [[42.626586, 62.273], [33.355586, 62.273]],
        [[58.628586, 66.337], [58.628586, 62.273]],
        [[163.957, 62.273], [254.771586, 62.273]],
        [[42.626586, 62.273], [42.626586, 66.337]],
        [[33.355586, 114.555], [33.355586, 62.273]],
        [[163.957, 63.4238], [163.957, 62.273]],
        [[33.355586, 114.555], [254.771586, 114.555]],
        [[157.861, 63.4238], [163.957, 63.4238]],
        [[42.626586, 66.337], [58.628586, 66.337]],
        [[254.771586, 114.555], [254.771586, 62.273]],
        [[157.861, 62.273], [157.861, 63.4238]],
    ],
    kicad_screw_positions = [
        [108.503586, 89.451],
        [72.943586, 89.451],
        [215.183586, 89.451],
        [144.063586, 89.451],
        [179.623586, 89.451],
        [250.743586, 89.451],
        [37.383586, 89.451],
    ],
    kicad_wire_relief_positions = [
        [182.16, 103.375],
        [75.46329, 103.425],
    ],
    fillet_keys = false,
    pcb_silkscreen_path = "../images/scout-2-brd-d72051a.svg",
    show_base = true,
    show_battery_holder = true,
    show_speaker = true,
    show_pcb = true,
    show_pcb_silkscreen = false,
    show_mounting_rail = true,
    show_keys = true
);