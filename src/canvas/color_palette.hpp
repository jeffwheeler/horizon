#pragma once

namespace horizon {
enum class ColorP {
    FROM_LAYER,
    JUNCTION,
    FRAG_ORPHAN,
    AIRWIRE_ROUTER,
    TEXT_OVERLAY,
    HOLE,
    DIMENSION,
    ERROR,
    NET,
    BUS,
    FRAME,
    AIRWIRE,
    PIN,
    PIN_HIDDEN,
    DIFFPAIR,
    N_COLORS,
    // colors after N_COLORS aren't part of the UBO
    BACKGROUND,
    GRID,
    CURSOR_NORMAL,
    CURSOR_TARGET,
    ORIGIN,
    MARKER_BORDER,
    SELECTION_BOX,
    SELECTION_LINE,
    SELECTABLE_OUTER,
    SELECTABLE_INNER,
    SELECTABLE_PRELIGHT,
    SELECTABLE_ALWAYS,
};
}
