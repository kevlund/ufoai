#include "OrthoContextMenu.h"
#include "EntityClassChooser.h"
#include "gtkutil/IconTextMenuItem.h"

#include "iselection.h"
#include "../../sidebar/surfaceinspector.h" // SurfaceInspector_FitTexture()
#include "../../entity.h" // Entity_createFromSelection(), Entity_connectSelected()
#include "gtkutil/dialog.h"
#include "radiant_i18n.h"

namespace ui
{
	namespace
	{
		/* CONSTANTS */
		const char* LIGHT_CLASSNAME = "light";
		const char* MODEL_CLASSNAME = "misc_model";

		const char* ADD_MODEL_TEXT = _("Create model...");
		const char* ADD_MODEL_ICON = "cmenu_add_model.png";
		const char* ADD_LIGHT_TEXT = _("Create light...");
		const char* ADD_LIGHT_ICON = "cmenu_add_light.png";
		const char* ADD_ENTITY_TEXT = _("Create entity...");
		const char* ADD_ENTITY_ICON = "cmenu_add_entity.png";
		const char* CONNECT_ENTITIES_TEXT = _("Connect entities...");
		const char* CONNECT_ENTITIES_ICON = "cmenu_connect_entities.png";
		const char* FIT_TEXTURE_TEXT = _("Fit textures...");
		const char* FIT_TEXTURE_ICON = "cmenu_fit_texture.png";
	}

	// Static class function to display the singleton instance.
	void OrthoContextMenu::displayInstance (const Vector3& point)
	{
		static OrthoContextMenu _instance;
		_instance.show(point);
	}

	// Constructor. Create GTK widgets here.
	OrthoContextMenu::OrthoContextMenu () :
		_widget(gtk_menu_new())
	{
		GtkWidget* addModel = gtkutil::IconTextMenuItem(ADD_MODEL_ICON, ADD_MODEL_TEXT);
		GtkWidget* addLight = gtkutil::IconTextMenuItem(ADD_LIGHT_ICON, ADD_LIGHT_TEXT);
		GtkWidget* addEntity = gtkutil::IconTextMenuItem(ADD_ENTITY_ICON, ADD_ENTITY_TEXT);

		// Context sensitive menu items
		_connectEntities = gtkutil::IconTextMenuItem(CONNECT_ENTITIES_ICON, CONNECT_ENTITIES_TEXT);
		_fitTexture = gtkutil::IconTextMenuItem(FIT_TEXTURE_ICON, FIT_TEXTURE_TEXT);

		g_signal_connect(G_OBJECT(addEntity), "activate", G_CALLBACK(callbackAddEntity), this);
		g_signal_connect(G_OBJECT(addLight), "activate", G_CALLBACK(callbackAddLight), this);
		g_signal_connect(G_OBJECT(addModel), "activate", G_CALLBACK(callbackAddModel), this);
		g_signal_connect(G_OBJECT(_connectEntities), "activate", G_CALLBACK(callbackConnectEntities), this);
		g_signal_connect(G_OBJECT(_fitTexture), "activate", G_CALLBACK(callbackFitTexture), this);

		gtk_menu_shell_append(GTK_MENU_SHELL(_widget), addModel);
		gtk_menu_shell_append(GTK_MENU_SHELL(_widget), addLight);
		gtk_menu_shell_append(GTK_MENU_SHELL(_widget), addEntity);
		gtk_menu_shell_append(GTK_MENU_SHELL(_widget), gtk_separator_menu_item_new());
		gtk_menu_shell_append(GTK_MENU_SHELL(_widget), _connectEntities);
		gtk_menu_shell_append(GTK_MENU_SHELL(_widget), _fitTexture);

		gtk_widget_show_all(_widget);
	}

	// Show the menu
	void OrthoContextMenu::show (const Vector3& point)
	{
		_lastPoint = point;
		checkConnectEntities();
		checkFitTexture();
		gtk_menu_popup(GTK_MENU(_widget), NULL, NULL, NULL, NULL, 1, GDK_CURRENT_TIME);
	}

	void OrthoContextMenu::checkConnectEntities ()
	{
		int countSelected = GlobalSelectionSystem().countSelected();
		if (countSelected == 2) {
			gtk_widget_set_sensitive(_connectEntities, TRUE);
		} else {
			gtk_widget_set_sensitive(_connectEntities, FALSE);
		}
	}

	void OrthoContextMenu::checkFitTexture ()
	{
		int countSelected = GlobalSelectionSystem().countSelected();
		if (countSelected > 0) {
			gtk_widget_set_sensitive(_fitTexture, TRUE);
		} else {
			gtk_widget_set_sensitive(_fitTexture, FALSE);
		}
	}

	/* GTK CALLBACKS */

	void OrthoContextMenu::callbackConnectEntities (GtkMenuItem* item, OrthoContextMenu* self)
	{
		Entity_connectSelected();
	}

	void OrthoContextMenu::callbackFitTexture (GtkMenuItem* item, OrthoContextMenu* self)
	{
		SurfaceInspector_FitTexture();
	}

	void OrthoContextMenu::callbackAddEntity (GtkMenuItem* item, OrthoContextMenu* self)
	{
		EntityClassChooser::displayInstance(self->_lastPoint);
	}

	void OrthoContextMenu::callbackAddLight (GtkMenuItem* item, OrthoContextMenu* self)
	{
		Entity_createFromSelection(LIGHT_CLASSNAME, self->_lastPoint);
	}

	void OrthoContextMenu::callbackAddModel (GtkMenuItem* item, OrthoContextMenu* self)
	{
		Entity_createFromSelection(MODEL_CLASSNAME, self->_lastPoint);
	}

} // namespace ui
