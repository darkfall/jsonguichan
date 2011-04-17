//
//  XmlGui - json parsing class for guichan (http://guichan.sourceforge.net), use jsoncpp for parse json script (http://jsoncpp.sourceforge.net).
//  author - darkfall (darkfall3@gmail.com)
//  website - https://github.com/darkfall/jsonguichan or http://code.google.com/p/jsonguichan/
//  blog - http://www.gamemastercn.com
//
//  jsongui.cpp
//  Sora
//
//  Created by Griffin Bu on 3/28/11.
//  Copyright 2011 Ruiwei Bu. All rights reserved.
//

#include "jsongui.h"

namespace gcn {
    
    JsonGui::JsonGui() {
    }
    
    bool JsonGui::parse(const std::string& filePath) {
        if(reader.parse(filePath.c_str(), rootValue)) {
            parseWidget(rootValue, NULL);
            return true;
        }
        return false;
    }
    
    bool JsonGui::parse(void* data, unsigned long size) {
        const char* pstrdata = (const char*)data;
        if(reader.parse(pstrdata, pstrdata+size, rootValue)) {
            parseWidget(rootValue, NULL);
            return true;
        }
        return false;
    }
    
    void JsonGui::addToParent(gcn::Widget* widget, gcn::Widget* parent) {
		if(parent == NULL) return;
		
        gcn::Container* pcont = dynamic_cast<gcn::Container*>(parent);
        if(pcont) {
            pcont->add(widget);
        } else {
            gcn::Window* pwnd = dynamic_cast<gcn::Window*>(parent);
            if(pwnd) {
                pwnd->add(widget);
            } else {
                gcn::ScrollArea* pscroll = dynamic_cast<gcn::ScrollArea*>(parent);
                if(pscroll) {
                    pscroll->setContent(widget);
                }
            }
        }
    }
    
    gcn::Widget* JsonGui::getWidget(const std::string& name) const {
        WIDGET_MAP::const_iterator itWidget = widgets.find(name);
        if(itWidget != widgets.end()) {
            return itWidget->second;
        }
        return NULL;
    }
    
    void JsonGui::registerParseFunc(const std::string& name, JsonGuiParseFunc func) {
        extensionFuncs[name] = func;
    }
	
	void JsonGui::_parseWidget(const char* name, const Json::Value& val, gcn::Widget* parent) {
		if(strcmp(name, "container") == 0) {
			parseContainer(val, parent);
		} else if(strcmp(name, "window") == 0) {
			parseWindow(val, parent);
		} else if(strcmp(name, "button") == 0) {
			parseButton(val, parent);
		} else if(strcmp(name, "slider") == 0) {
			parseSlider(val, parent);
		} else if(strcmp(name, "textbox") == 0) {
			parseTextBox(val, parent);
		} else if(strcmp(name, "textfield") == 0) {
			parseTextField(val, parent);
		} else if(strcmp(name, "label") == 0) {
			parseLabel(val, parent);
		} else if(strcmp(name, "checkbox") == 0) {
			parseCheckbox(val, parent);
		} else if(strcmp(name, "radiobutton") == 0) {
			parseRadioButton(val, parent);
		} else if(strcmp(name, "imagebutton") == 0) {
			parseImageButton(val, parent);
		} else if(strcmp(name, "icon") == 0) {
			parseIcon(val, parent);
		} else if(strcmp(name, "listbox") == 0) {
			parseListBox(val, parent);
		} else if(strcmp(name, "dropdown") == 0) {
			parseDropDown(val, parent);
		}
		else {
			EXTENSION_FUNC_MAP::iterator itFunc = extensionFuncs.find(name);
			if(itFunc != extensionFuncs.end()) {
				itFunc->second(val, parent, this);
			}
		}
	}
    
    void JsonGui::parseWidget(const Json::Value& val, gcn::Widget* parent) {
        VALUE_NAMES members = val.getMemberNames();
        VALUE_NAMES::iterator itMember = members.begin();
        while(itMember != members.end()) {
            const char* type = itMember->c_str();

            bool isObjectValue = val[type].isObject();
            if(isObjectValue) {
				_parseWidget(type, val[type], parent);
			} else if(val[type].isArray()) {
				if(strcmp(type, "widgets") == 0) {
					for(int i=0; i<val[type].size(); ++i) {
						if(val[type][i].isObject()) {
							parseWidget(val[type][i], parent);
						}
					}
				}
			}
            ++itMember;
        }

    }
    
    void JsonGui::parseDefaults(const Json::Value& val, gcn::Widget* widget) {
        if(val.isMember("x")) {
            widget->setX(val["x"].asInt());
        }
        if(val.isMember("y")) {
            widget->setY(val["y"].asInt());
        }
        if(val.isMember("width")) {
            widget->setWidth(val["width"].asInt());
        }
        if(val.isMember("height")) {
            widget->setHeight(val["height"].asInt());
        }
        if(val.isMember("basecolor")) {
            int color;
            sscanf(val["basecolor"].asCString(), "%x", &color);
            widget->setBaseColor(gcn::Color(color));
        }
        if(val.isMember("foregroundcolor")) {
            int color;
            sscanf(val["foregroundcolor"].asCString(), "%x", &color);
            widget->setForegroundColor(gcn::Color(color));
        }
        if(val.isMember("backgroundcolor")) {
            int color;
            sscanf(val["backgroundcolor"].asCString(), "%x", &color);
            widget->setBackgroundColor(gcn::Color(color));
        }
        
        if(val.isMember("framesize")) {
            widget->setFrameSize(val["framesize"].asInt());
        }
        
       /* if(val.isMember("font")) {
            int fontsize;
            if(val.isMember("fontsize")) 
                fontsize = val["fontsize"].asInt();
            else fontsize = 16;	
            gcn::SoraGUIFont* pfont = new gcn::SoraGUIFont(val["font"].asCString(), fontsize);
            if(pfont) {
                widget->setFont(pfont);
            }
        }*/
        
        if(val.isMember("visible"))
            widget->setVisible(val["visible"].asBool());
        
        if(val.isMember("focusable"))
            widget->setFocusable(val["focusable"].asBool());
        
        if(val.isMember("enabled"))
            widget->setEnabled(val["enabled"].asBool());
        
        if(val.isMember("tabin"))
            widget->setTabInEnabled(val["tabin"].asBool());
        
        if(val.isMember("tabout"))
            widget->setTabOutEnabled(val["tabout"].asBool());
        
        if(val.isMember("eventId"))
            widget->setActionEventId(val["eventId"].asString());
        
        if(val.isMember("id")) {
            widget->setId(val["id"].asString());
           /* if(val.isMember("responser")) {
                SoraString arg = val["responser"].asString();
                SoraString type = val["responsetype"].asString();
                parseResponser(widget, arg, type.size()==0?NULL:&type);
            }*/
        }
        /*
        if(val.isMember("parent")) {
            if(widget->getParent() == NULL)
                GCN_GLOBAL->addWidget(widget, val["parent"].asString());
        }*/
    }
    
    void JsonGui::parseContainer(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Container Widget must have a unique name");
        }
        
        gcn::Container *c = new gcn::Container();
        
        if(val.isMember("opaque"))
            c->setOpaque(val["opaque"].asBool());
        
        parseDefaults(val, c);
        
        //parsing child elements
		if(val.isMember("widgets")) {
			Json::Value widgets = val["widgets"];
			if(widgets.isArray()) {
				for(int i=0; i<widgets.size(); ++i) {
					if(widgets[i].isObject()) {
						parseWidget(widgets[i], c);
					}
				}
			}
        }
		
        addToParent(c, parent);
        widgets[name] = c;
    }
    
    void JsonGui::parseWindow(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Window Widget must have a unique name");
        }
        
        gcn::Window *window = new gcn::Window;
        if(val.isMember("caption")) {
            window->setCaption(val["caption"].asString());
        }
        if(val.isMember("tabbing")) {
            window->setPadding(val["tabbing"].asBool());
        }
        if(val.isMember("movable")) {
            window->setMovable(val["movable"].asBool());
        }
        if(val.isMember("titleBarHeight")) {
            window->setTitleBarHeight(val["titleBarHeight"].asInt());
        }
        
        if(val.isMember("opaque"))
            window->setOpaque(val["opaque"].asBool());
        
        
        parseDefaults(val, window);
        
        //parsing child elements
		if(val.isMember("widgets")) {
			Json::Value widgets = val["widgets"];
			if(widgets.isArray()) {
				for(int i=0; i<widgets.size(); ++i) {
					if(widgets[i].isObject()) {
						parseWidget(widgets[i], window);
					}
				}
			}
        }
                
        addToParent(window, parent);
        widgets[name] = window;
    }
    
    void JsonGui::parseButton(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Button Widget must have a unique name");
        }
        
        gcn::Button *button = new gcn::Button;
        if(val.isMember("caption")) {
            button->setCaption(val["caption"].asString());
        }
        if(val.isMember("align")) {
            const char* align = val["align"].asCString();
            if(strcmp(align, "center")) {
                button->setAlignment(gcn::Graphics::CENTER);
            } else if(strcmp(align, "left")) {
                button->setAlignment(gcn::Graphics::LEFT);
            } else {
                button->setAlignment(gcn::Graphics::RIGHT);
            }
        }
        
        button->adjustSize();
        parseDefaults(val, button);
		
        addToParent(button, parent);
        widgets[name] = button;
    }
    
    void JsonGui::parseSlider(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
        
        gcn::Slider *slider = new gcn::Slider;
        if(val.isMember("start")) {            
            slider->setScaleStart(val["start"].asDouble());
        }
        if(val.isMember("end")) {
            slider->setScaleEnd(val["end"].asDouble());
        }
        if(val.isMember("value")) {
            slider->setValue(val["value"].asDouble());
        }
        if(val.isMember("markerLength")) {
            slider->setMarkerLength(val["markerLength"].asInt());
        }
        if(val.isMember("stepLength")) {
            slider->setStepLength(val["stepLength"].asDouble());
        }
        if(val.isMember("orientation")) {
            if(strcmp(val["orientation"].asCString(), "horizontal"))
                slider->setOrientation(gcn::Slider::HORIZONTAL);
            else slider->setOrientation(gcn::Slider::VERTICAL);
        }
        
        parseDefaults(val, slider);
        
        addToParent(slider, parent);
        widgets[name] = slider;
    }
    
    void JsonGui::parseLabel(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Icon Widget must have a unique name");
        }
        
        gcn::Label *label = new gcn::Label;
        if(val.isMember("caption")) {
            label->setCaption(val["caption"].asString());
        }
        
        label->adjustSize();
        if(val.isMember("align")) {
            const char* align = val["align"].asCString();
            if(strcmp(align, "center")) {
                label->setAlignment(gcn::Graphics::CENTER);
            } else if(strcmp(align, "left")) {
                label->setAlignment(gcn::Graphics::LEFT);
            } else {
                label->setAlignment(gcn::Graphics::RIGHT);
            }
        }
        
        parseDefaults(val, label);
        
        addToParent(label, parent);
        widgets[name] = label;
    }
    
    void JsonGui::parseImageButton(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Icon Widget must have a unique name");
        }
        
        gcn::ImageButton* pButton = 0;
        gcn::Image* image;
        
        if(val.isMember("image")) {
            image = gcn::Image::load(val["image"].asString());
        }
        if(image) {
            pButton = new gcn::ImageButton(image);
            parseDefaults(val, pButton);
            
            addToParent(pButton, parent);
            widgets[name] = pButton;
        }
    }
    
    void JsonGui::parseIcon(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Icon Widget must have a unique name");
        }
        
        gcn::Icon *icon;
        gcn::Image *image;
        
        if(val.isMember("image")) {
            image = gcn::Image::load(val["image"].asString());
        }
        
        if(image) {
            icon = new gcn::Icon(image);
            
            parseDefaults(val, icon);
            
            addToParent(icon, parent);
            widgets[name] = icon;
        }
    }
    
    void JsonGui::parseCheckbox(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
        
        gcn::CheckBox *checkbox = new gcn::CheckBox;
        
        if(val.isMember("caption")) {
            checkbox->setCaption(val["caption"].asString());
        }
        
        checkbox->adjustSize();
    
        parseDefaults(val, checkbox);

        addToParent(checkbox, parent);
        widgets[name] = checkbox;
    }
    
    void JsonGui::parseTextBox(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
        
        gcn::TextBox *textbox = new gcn::TextBox;
        
        if(val.isMember("editable"))
            textbox->setEditable(val["editable"].asBool());
        
        if(val.isMember("text")) {
            textbox->setText(val["text"].asString());
        }
        
        if(val.isMember("opaque"))
            textbox->setOpaque(val["opaque"].asBool());
        
        parseDefaults(val, textbox);
        
        addToParent(textbox, parent);
        widgets[name] = textbox;
    }
    
    void JsonGui::parseTextField(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
        
        gcn::TextField *textField = new gcn::TextField;
    
        if(val.isMember("text")) {
            textField->setText(val["text"].asString());
        }
    
        parseDefaults(val, textField);
        
        addToParent(textField, parent);
        widgets[name] = textField;
    }
    
    void JsonGui::parseRadioButton(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
        
        gcn::RadioButton *radio = new gcn::RadioButton;
        
        if(val.isMember("caption")) {
            radio->setCaption(val["caption"].asString());
        }
        radio->adjustSize();
        
        if(val.isMember("group"))
            radio->setGroup(val["group"].asString());		
        
        parseDefaults(val, radio);
        
        addToParent(radio, parent);
        widgets[name] = radio;
    }
	
	void JsonGui::parseListBox(const Json::Value& val, gcn::Widget* parent) {
		std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
		
		gcn::ListBox* listbox = new gcn::ListBox;
		if(val.isMember("selectioncolor")) {
			int color;
            sscanf(val["selectioncolor"].asCString(), "%x", &color);
            listbox->setSelectionColor(gcn::Color(color));
		}
		
		if(val.isMember("items")) {
			Json::Value items = val["items"];
			if(items.isArray()) {
				JsonListModel* listModel = new JsonListModel;

				for(int i=0; i<items.size(); ++i) {
					listModel->pushElement(items[i].asString());
				}
				listbox->setListModel(listModel);
			}
		}
		
		parseDefaults(val, listbox);
		addToParent(listbox, parent);
		widgets[name] = listbox;
	}
	
	void JsonGui::parseDropDown(const Json::Value& val, gcn::Widget* parent) {
		std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        } else {
            throw GCN_EXCEPTION("Slider Widget must have a unique name");
        }
		
		gcn::ListBox* dropdown = new gcn::ListBox;
		if(val.isMember("selectioncolor")) {
			int color;
            sscanf(val["selectioncolor"].asCString(), "%x", &color);
            dropdown->setSelectionColor(gcn::Color(color));
		}
		
		if(val.isMember("items")) {
			Json::Value items = val["items"];
			if(items.isArray()) {
				JsonListModel* listModel = new JsonListModel;
				
				for(int i=0; i<items.size(); ++i) {
					listModel->pushElement(items[i].asString());
				}
				dropdown->setListModel(listModel);
			} 
		}
		
		parseDefaults(val, dropdown);
		addToParent(dropdown, parent);
		widgets[name] = dropdown;
	}
} // namespace sora