#include <gtk/gtk.h>


/*
 Function declaration
*/
static void on_about(GSimpleAction *simple, GVariant *parameter, gpointer user_data);
static void on_app_activate(GtkApplication *app, gpointer user_data);
static void on_app_startup(GApplication *app, gpointer user_data);
static void on_quit(GSimpleAction *simple, GVariant *parameter, gpointer user_data);



/* Callback function for the response signal "activate" related to the SimpleAction
 * "about_action".
 * This function is used to cause the about dialog window to popup.
 */
static void on_about(GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
  GtkWidget *about_dialog;

  about_dialog = gtk_about_dialog_new ();
  gtk_window_set_transient_for(GTK_WINDOW(about_dialog),GTK_WINDOW(user_data));

  /* Lists of authors/ documentators to be used later, they must be initialized
   * in a null terminated array of strings.
   */
  const gchar *authors[] = {"Laurent LAMARD","Benjamin LAMARD","Laurine LAMARD", NULL};
  const gchar *documenters[] = {"GNOME Documentation Team", NULL};

  /* We fill in the information for the about dialog */
  gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG (about_dialog), "Tuto GTK+ 3.0");
  gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (about_dialog), "Copyright \xc2\xa9 2016 Laurent LAMARD for GNOME Documentation Team");
  gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (about_dialog), authors);
  gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (about_dialog),"Version 0.1");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG (about_dialog),"Mon Tuto GTK3");
  gtk_about_dialog_set_documenters (GTK_ABOUT_DIALOG (about_dialog), documenters);
  gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG (about_dialog), "GNOME Developer Website");
  gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (about_dialog), "http://developer.gnome.org");
  gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG (about_dialog),"eog");

  /* We do not wish to show the title, which in this case would be
   * "AboutDialog Example". We have to reset the title of the messagedialog
   * window after setting the program name.
   */
  gtk_window_set_title (GTK_WINDOW (about_dialog), "");

  /* Show the about dialog */
  gtk_dialog_run (GTK_DIALOG(about_dialog));
/* Destroy the dialog */
  gtk_widget_destroy (GTK_WIDGET (about_dialog));
}

static void on_app_activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;

  GSimpleAction *about_action;

  /* Create a window with a title and a default size */
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Tuto GTK3");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

  /* Create a new simple action, giving it a NULL parameter type. It will
   * always be NULL for actions invoked from a menu. (e.g clicking on an "ok"
   * or "cancel" button)
   */
  about_action = g_simple_action_new ("about", NULL);

  /* Connect the "activate" signal to the appropriate callback function.
   * It will indicate that the action was just activated.
   */
  g_signal_connect (about_action, "activate", G_CALLBACK (on_about), GTK_WINDOW (window));

  /* Adds the about_action to the overall action map. An Action map is an
   * interface that contains a number of named GAction instances
   * (such as about_action)
   */
  g_action_map_add_action (G_ACTION_MAP (window), G_ACTION (about_action));

  gtk_widget_show_all (window);
}

/* Callback function for the response signal "activate" from the "quit" action
 * found in the function directly below.
 */
static void on_quit(GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
  GApplication *application = user_data;
  g_application_quit (application);
}

/* Startup function for the menu we are creating in this sample */
static void on_app_startup(GApplication *app, gpointer user_data)
{
  GSimpleAction *quit_action;

  /* Initialize the application menu, and add a menu item with label "About" and action
   * "win.about". Also add another menu item with label "Quit" and action
   * "app.quit"
   */

  /* Create a new simple action for the application. (In this case it is the
   * "quit" action.
   */
  quit_action = g_simple_action_new ("quit", NULL);

  /* Ensure that the menu we have just created is set for the overall application */

  g_signal_connect (quit_action,"activate", G_CALLBACK (on_quit),app);
  g_action_map_add_action (G_ACTION_MAP (app), G_ACTION (quit_action));
  GtkBuilder *builder = gtk_builder_new ();

  gtk_builder_add_from_string (builder,
                               "<interface>" 
                             "  <menu id='app-menu'>"
                               "    <section>"
                               "      <item>"
                               "        <attribute name='label' translatable='yes'>_About</attribute>"
                               "        <attribute name='action'>win.about</attribute>"
                               "      </item>"
                               "      <item>"
                               "        <attribute name='label' translatable='yes'>_Quit</attribute>"
                               "        <attribute name='action'>app.quit</attribute>"
                               "        <attribute name='accel'>&lt;Primary&gt;q</attribute>"
                               "      </item>"
                               "    </section>"
                               "  </menu>"                
				"<menu id='menubar'>"		     
				"    <submenu>"
				"    <attribute name='label' translatable='yes'>_File</attribute>"
				"      <item>"
				"        <attribute name='label' translatable='yes'>_Quit</attribute>"
                                "        <attribute name='action'>app.quit</attribute>"
				"      </item>"
				"    </submenu>" 
				"    <submenu>"
				"    <attribute name='label' translatable='yes'>_Help</attribute>"
				"      <item>"
				"        <attribute name='label' translatable='yes'>_About</attribute>"
                                "        <attribute name='action'>win.about</attribute>"
				"      </item>"
				"    </submenu>" 
				"</menu>"
                               "</interface>", -1, NULL);

/*
   setup application menu and window menu
*/
  gtk_application_set_app_menu(GTK_APPLICATION (app), G_MENU_MODEL(gtk_builder_get_object (builder, "app-menu")));
  gtk_application_set_menubar(GTK_APPLICATION (app),G_MENU_MODEL(gtk_builder_get_object (builder, "menubar")));

  g_object_unref (builder);
  

}

/* Main function */
int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("tuto.gtk.lla", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (on_app_activate), NULL);
  g_signal_connect (app, "startup", G_CALLBACK (on_app_startup), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
