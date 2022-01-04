//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     track_data data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

#include <optional>
#include <stdexcept>
#include <regex>

namespace track {
    using nlohmann::json;

    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }

    class color {
        public:
        color() = default;
        virtual ~color() = default;

        private:
        int64_t blue;
        int64_t green;
        std::string id;
        int64_t red;

        public:
        const int64_t & get_blue() const { return blue; }
        int64_t & get_mutable_blue() { return blue; }
        void set_blue(const int64_t & value) { this->blue = value; }

        const int64_t & get_green() const { return green; }
        int64_t & get_mutable_green() { return green; }
        void set_green(const int64_t & value) { this->green = value; }

        const std::string & get_id() const { return id; }
        std::string & get_mutable_id() { return id; }
        void set_id(const std::string & value) { this->id = value; }

        const int64_t & get_red() const { return red; }
        int64_t & get_mutable_red() { return red; }
        void set_red(const int64_t & value) { this->red = value; }
    };

    class junction {
        public:
        junction() = default;
        virtual ~junction() = default;

        private:
        std::vector<int64_t> children;
        int64_t parent;
        int64_t selected;

        public:
        const std::vector<int64_t> & get_children() const { return children; }
        std::vector<int64_t> & get_mutable_children() { return children; }
        void set_children(const std::vector<int64_t> & value) { this->children = value; }

        const int64_t & get_parent() const { return parent; }
        int64_t & get_mutable_parent() { return parent; }
        void set_parent(const int64_t & value) { this->parent = value; }

        const int64_t & get_selected() const { return selected; }
        int64_t & get_mutable_selected() { return selected; }
        void set_selected(const int64_t & value) { this->selected = value; }
    };

    class point {
        public:
        point() = default;
        virtual ~point() = default;

        private:
        int64_t x;
        int64_t y;

        public:
        const int64_t & get_x() const { return x; }
        int64_t & get_mutable_x() { return x; }
        void set_x(const int64_t & value) { this->x = value; }

        const int64_t & get_y() const { return y; }
        int64_t & get_mutable_y() { return y; }
        void set_y(const int64_t & value) { this->y = value; }
    };

    class track {
        public:
        track() = default;
        virtual ~track() = default;

        private:
        std::vector<int64_t> points;

        public:
        const std::vector<int64_t> & get_points() const { return points; }
        std::vector<int64_t> & get_mutable_points() { return points; }
        void set_points(const std::vector<int64_t> & value) { this->points = value; }
    };

    class track_data {
        public:
        track_data() = default;
        virtual ~track_data() = default;

        private:
        std::vector<color> colors;
        std::vector<junction> junctions;
        std::vector<point> points;
        std::vector<track> tracks;
        std::string version;

        public:
        const std::vector<color> & get_colors() const { return colors; }
        std::vector<color> & get_mutable_colors() { return colors; }
        void set_colors(const std::vector<color> & value) { this->colors = value; }

        const std::vector<junction> & get_junctions() const { return junctions; }
        std::vector<junction> & get_mutable_junctions() { return junctions; }
        void set_junctions(const std::vector<junction> & value) { this->junctions = value; }

        const std::vector<point> & get_points() const { return points; }
        std::vector<point> & get_mutable_points() { return points; }
        void set_points(const std::vector<point> & value) { this->points = value; }

        const std::vector<track> & get_tracks() const { return tracks; }
        std::vector<track> & get_mutable_tracks() { return tracks; }
        void set_tracks(const std::vector<track> & value) { this->tracks = value; }

        const std::string & get_version() const { return version; }
        std::string & get_mutable_version() { return version; }
        void set_version(const std::string & value) { this->version = value; }
    };
}

namespace nlohmann {
    void from_json(const json & j, track::color & x);
    void to_json(json & j, const track::color & x);

    void from_json(const json & j, track::junction & x);
    void to_json(json & j, const track::junction & x);

    void from_json(const json & j, track::point & x);
    void to_json(json & j, const track::point & x);

    void from_json(const json & j, track::track & x);
    void to_json(json & j, const track::track & x);

    void from_json(const json & j, track::track_data & x);
    void to_json(json & j, const track::track_data & x);

    inline void from_json(const json & j, track::color& x) {
        x.set_blue(j.at("blue").get<int64_t>());
        x.set_green(j.at("green").get<int64_t>());
        x.set_id(j.at("id").get<std::string>());
        x.set_red(j.at("red").get<int64_t>());
    }

    inline void to_json(json & j, const track::color & x) {
        j = json::object();
        j["blue"] = x.get_blue();
        j["green"] = x.get_green();
        j["id"] = x.get_id();
        j["red"] = x.get_red();
    }

    inline void from_json(const json & j, track::junction& x) {
        x.set_children(j.at("children").get<std::vector<int64_t>>());
        x.set_parent(j.at("parent").get<int64_t>());
        x.set_selected(j.at("selected").get<int64_t>());
    }

    inline void to_json(json & j, const track::junction & x) {
        j = json::object();
        j["children"] = x.get_children();
        j["parent"] = x.get_parent();
        j["selected"] = x.get_selected();
    }

    inline void from_json(const json & j, track::point& x) {
        x.set_x(j.at("x").get<int64_t>());
        x.set_y(j.at("y").get<int64_t>());
    }

    inline void to_json(json & j, const track::point & x) {
        j = json::object();
        j["x"] = x.get_x();
        j["y"] = x.get_y();
    }

    inline void from_json(const json & j, track::track& x) {
        x.set_points(j.at("points").get<std::vector<int64_t>>());
    }

    inline void to_json(json & j, const track::track & x) {
        j = json::object();
        j["points"] = x.get_points();
    }

    inline void from_json(const json & j, track::track_data& x) {
        x.set_colors(j.at("colors").get<std::vector<track::color>>());
        x.set_junctions(j.at("junctions").get<std::vector<track::junction>>());
        x.set_points(j.at("points").get<std::vector<track::point>>());
        x.set_tracks(j.at("tracks").get<std::vector<track::track>>());
        x.set_version(j.at("version").get<std::string>());
    }

    inline void to_json(json & j, const track::track_data & x) {
        j = json::object();
        j["colors"] = x.get_colors();
        j["junctions"] = x.get_junctions();
        j["points"] = x.get_points();
        j["tracks"] = x.get_tracks();
        j["version"] = x.get_version();
    }
}
