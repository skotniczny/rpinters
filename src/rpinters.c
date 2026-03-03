/*============================================================================
Copyright (c) 2025 Raspberry Pi
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
============================================================================*/

#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <locale.h>

#include "cc-printers-panel.h"

/*----------------------------------------------------------------------------*/
/* Typedefs and macros                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global data                                                                */
/*----------------------------------------------------------------------------*/

static CcPrintersPanel *cpp;
#ifndef PLUGIN_NAME
static GtkBuilder *builder;
static GtkWidget *main_dlg;
#endif

/*----------------------------------------------------------------------------*/
/* Prototypes                                                                 */
/*----------------------------------------------------------------------------*/

#ifndef PLUGIN_NAME
static gboolean ok_main (GtkButton *button, gpointer data);
static gboolean close_prog (GtkWidget *widget, GdkEvent *event, gpointer data);
#endif

/*----------------------------------------------------------------------------*/
/* Function definitions                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helpers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Initial configuration                                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Plugin interface */
/*----------------------------------------------------------------------------*/

#ifdef PLUGIN_NAME

void init_plugin (GtkWidget *)
{
    setlocale (LC_ALL, "");
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);

    system ("systemctl start cups-browsed");

    cpp = g_object_new (CC_TYPE_PRINTERS_PANEL, NULL);
}

int plugin_tabs (void)
{
    return 1;
}

const char *tab_name (int tab)
{
    return C_("tab", "Printers");
}

const char *icon_name (int tab)
{
    return "printer";
}

const char *tab_id (int tab)
{
    return NULL;
}

GtkWidget *get_tab (int tab)
{
    return GTK_WIDGET (cpp);
}

gboolean reboot_needed (void)
{
    system ("systemctl stop cups-browsed");

    return FALSE;
}

void free_plugin (void)
{
}

#else

/*----------------------------------------------------------------------------*/
/* Main window button handlers                                                */
/*----------------------------------------------------------------------------*/

static gboolean ok_main (GtkButton *button, gpointer data)
{
    gtk_main_quit ();
    return FALSE;
}

static gboolean close_prog (GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_main_quit ();
    return TRUE;
}

/*----------------------------------------------------------------------------*/
/* Main window                                                                */
/*----------------------------------------------------------------------------*/

int main (int argc, char *argv[])
{
    GtkWidget *wid;

    setlocale (LC_ALL, "");
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);

    main_dlg = NULL;
    gtk_init (&argc, &argv);
    
    cpp = g_object_new (CC_TYPE_PRINTERS_PANEL, NULL);

    builder = gtk_builder_new_from_file (PACKAGE_DATA_DIR "/ui/rpinters.ui");

    main_dlg = (GtkWidget *) gtk_builder_get_object (builder, "main_window");
    g_signal_connect (main_dlg, "delete_event", G_CALLBACK (close_prog), NULL);

    wid = (GtkWidget *) gtk_builder_get_object (builder, "button_ok");
    g_signal_connect (wid, "clicked", G_CALLBACK (ok_main), NULL);

    wid = (GtkWidget *) gtk_builder_get_object (builder, "box");
    gtk_box_pack_start (GTK_BOX (wid), GTK_WIDGET (cpp), TRUE, TRUE, 5);
    gtk_box_reorder_child (GTK_BOX (wid), GTK_WIDGET (cpp), 0);
    
    g_object_unref (builder);

    gtk_widget_show (main_dlg);

    gtk_main ();

    return 0;
}

#endif

/* End of file */
/*----------------------------------------------------------------------------*/
