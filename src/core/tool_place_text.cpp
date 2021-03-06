#include "tool_place_text.hpp"
#include "imp/imp_interface.hpp"
#include <iostream>
#include "nlohmann/json.hpp"

namespace horizon {

void ToolPlaceText::Settings::load_from_json(const json &j)
{
    width = j.value("width", 0);
    size = j.value("size", 1.5_mm);
}

json ToolPlaceText::Settings::serialize() const
{
    json j;
    j["width"] = width;
    j["size"] = size;
    return j;
}

ToolPlaceText::ToolPlaceText(Core *c, ToolID tid) : ToolBase(c, tid), ToolHelperMove(c, tid)
{
}

bool ToolPlaceText::can_begin()
{
    return core.r->has_object_type(ObjectType::TEXT);
}

void ToolPlaceText::apply_settings()
{
    if (temp) {
        temp->width = settings.width;
        temp->size = settings.size;
    }
}

ToolResponse ToolPlaceText::begin(const ToolArgs &args)
{
    std::cout << "tool place text\n";

    temp = core.r->insert_text(UUID::random());
    temp->layer = args.work_layer;
    temp->placement.shift = args.coords;
    apply_settings();
    imp->tool_bar_set_tip(
            "<b>LMB:</b>place text <b>RMB:</b>finish "
            "<b>space:</b>change text <b>w:</b>text width <b>s:</b>text size");

    auto r = imp->dialogs.ask_datum_string("Enter text", temp->text);
    if (r.first) {
        temp->text = r.second;
    }
    else {
        core.r->delete_text(temp->uuid);
        core.r->selection.clear();
        return ToolResponse::end();
    }
    core.r->selection.emplace(temp->uuid, ObjectType::TEXT);

    return ToolResponse();
}
ToolResponse ToolPlaceText::update(const ToolArgs &args)
{
    std::string *text;

    if (args.type == ToolEventType::MOVE) {
        temp->placement.shift = args.coords;
    }
    else if (args.type == ToolEventType::CLICK) {
        if (args.button == 1) {
            text = &temp->text;
            texts_placed.push_front(temp);
            auto old_text = temp;
            temp = core.r->insert_text(UUID::random());
            temp->text = *text;
            temp->layer = args.work_layer;
            temp->placement = old_text->placement;
            temp->placement.shift = args.coords;
            apply_settings();
            core.r->selection.clear();
            core.r->selection.emplace(temp->uuid, ObjectType::TEXT);
        }
        else if (args.button == 3) {
            core.r->delete_text(temp->uuid);
            core.r->selection.clear();
            for (auto it : texts_placed) {
                core.r->selection.emplace(it->uuid, ObjectType::TEXT);
            }
            core.r->commit();
            return ToolResponse::end();
        }
    }
    else if (args.type == ToolEventType::LAYER_CHANGE) {
        temp->layer = args.work_layer;
    }
    else if (args.type == ToolEventType::KEY) {
        if (args.key == GDK_KEY_space) {
            auto r = imp->dialogs.ask_datum_string("Enter text", temp->text);
            if (r.first) {
                temp->text = r.second;
            }
        }
        else if (args.key == GDK_KEY_w) {
            auto r = imp->dialogs.ask_datum("Enter width", settings.width);
            if (r.first) {
                settings.width = std::max(r.second, (int64_t)0);
                apply_settings();
            }
        }
        else if (args.key == GDK_KEY_s) {
            auto r = imp->dialogs.ask_datum("Enter size", settings.size);
            if (r.first) {
                settings.size = std::max(r.second, (int64_t)0);
                apply_settings();
            }
        }
        else if (args.key == GDK_KEY_r || args.key == GDK_KEY_e) {
            bool rotate = args.key == GDK_KEY_r;
            move_mirror_or_rotate(temp->placement.shift, rotate);
        }
        else if (args.key == GDK_KEY_Escape) {
            core.r->revert();
            return ToolResponse::end();
        }
    }
    return ToolResponse();
}
} // namespace horizon
