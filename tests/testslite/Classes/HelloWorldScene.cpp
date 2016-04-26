
#include "HelloWorldScene.h"
#include "kaguya/kaguya.hpp"

USING_NS_CC;

class A {
public:
    static void say(int v) {
        printf("v = %d\n", v);
    };
};

class B: public A {
};


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {
        return false;
    }

    kaguya::State lua;
    lua.openlibs();

    // open namespace "cc"
    auto cc = lua["cc"] = lua.newTable();

    cc["Ref"]
    .setClass(kaguya::ClassMetatable<Ref>()
              .addMemberFunction("retain", &Ref::retain)
              .addMemberFunction("release", &Ref::release)
              .addMemberFunction("getReferenceCount", &Ref::getReferenceCount));

    cc["Scheduler"]
    .setClass(kaguya::ClassMetatable<Scheduler, Ref>());

    cc["Director"]
    .setClass(kaguya::ClassMetatable<Director, Ref>()
              .addStaticFunction("getInstance", &Director::getInstance)
              .addMemberFunction("endDirector", &Director::end)
              .addMemberFunction("getScheduler", &Director::getScheduler));

    cc["Node"]
    .setClass(kaguya::ClassMetatable<Node, Ref>()
              .addStaticFunction("create", &Node::create)
              .addMemberFunction("addChild", static_cast<void(Node::*)(Node*)>(&Node::addChild))
              .addMemberFunction("addChild", static_cast<void(Node::*)(Node*, int)>(&Node::addChild))
              .addMemberFunction("addChild", static_cast<void(Node::*)(Node*, int, int)>(&Node::addChild))
              .addMemberFunction("addChild", static_cast<void(Node::*)(Node*, int, const std::string&)>(&Node::addChild))
              .addMemberFunction("removeChild", &Node::removeChild)
              .addMemberFunction("setPosition", static_cast<void(Node::*)(const Vec2&)>(&Node::setPosition))
              .addMemberFunction("setPosition", static_cast<void(Node::*)(float, float)>(&Node::setPosition))
              .addMemberFunction("setColor", &Node::setColor)
              .addMemberFunction("setOpacity", &Node::setOpacity)
              .addMemberFunction("schedule", static_cast<void(Node::*)(const std::function<void(float)>&, float, const std::string &)>(&Node::schedule)));

    cc["Sprite"]
    .setClass(kaguya::ClassMetatable<Sprite, Node>()
              .addStaticFunction("create", static_cast<Sprite*(*)()>(&Sprite::create))
              .addStaticFunction("create", static_cast<Sprite*(*)(const std::string&)>(&Sprite::create)));

    cc["Layer"]
    .setClass(kaguya::ClassMetatable<Layer, Node>());

    cc["LayerColor"]
    .setClass(kaguya::ClassMetatable<LayerColor, Layer>()
              .addStaticFunction("create", static_cast<LayerColor*(*)()>(&LayerColor::create))
              .addStaticFunction("create", static_cast<LayerColor*(*)(const Color4B&, GLfloat, GLfloat)>(&LayerColor::create))
              .addStaticFunction("create", static_cast<LayerColor*(*)(const Color4B&)>(&LayerColor::create)));

    cc["Label"]
    .setClass(kaguya::ClassMetatable<Label, Node>()
              .addStaticFunction("create", &Label::create)
              .addStaticFunction("createWithSystemFont", [](const std::string& text, const std::string& font, float fontSize) -> Label* {
        return Label::createWithSystemFont(text, font, fontSize);
    })
              .addMemberFunction("setString", &Label::setString));

    cc["HelloWorld"]
    .setClass(kaguya::ClassMetatable<HelloWorld, Layer>());


    cc["Vec2"]
    .setClass(kaguya::ClassMetatable<Vec2>()
              .addConstructor<float, float>()
              .addConstructor<const Vec2&>()
              .addProperty("x", &Vec2::x)
              .addProperty("y", &Vec2::y));

    cc["Size"]
    .setClass(kaguya::ClassMetatable<Size>()
              .addConstructor<float, float>()
              .addConstructor<const Size&>()
              .addProperty("width", &Size::width)
              .addProperty("height", &Size::height));

    cc["Color4B"]
    .setClass(kaguya::ClassMetatable<Color4B>()
              .addConstructor()
              .addConstructor<GLubyte, GLubyte, GLubyte, GLubyte>()
              .addConstructor<const Color3B&, GLubyte>()
              .addConstructor<const Color3B&>()
              .addProperty("r", &Color4B::r)
              .addProperty("g", &Color4B::g)
              .addProperty("b", &Color4B::b)
              .addProperty("a", &Color4B::a));

    cc["Color3B"]
    .setClass(kaguya::ClassMetatable<Color3B>()
              .addConstructor()
              .addConstructor<GLubyte, GLubyte, GLubyte>()
              .addConstructor<const Color3B&>()
              .addConstructor<const Color4B&>()
              .addProperty("r", &Color3B::r)
              .addProperty("g", &Color3B::g)
              .addProperty("b", &Color3B::b));

    lua["g_viewsize"] = Director::getInstance()->getOpenGLView()->getFrameSize();
    lua["g_layer"] = this;
    lua.dofile("main.lua");

    return true;
}
