/* descq.c
       __
  ____/ /__  ______________ _
 / __  / _ \/ ___/ ___/ __ `/
/ /_/ /  __(__  ) /__/ /_/ /
\__,_/\___/____/\___/\__, /
                       /_/
Michael Leidel (Feb 2021)
OTHER FILES:
    descq.glade     programs GUI layout xml file
    data/*.*        data files for user's content ...
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
*/
#include "myclib.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <unistd.h>

#define EDITOR g_editor
#define BUFFER1 1024
#define BUFFER2 2048
#define BUFFER3 1048576
#define MAXURLS 200

// glade xml file
// prepared with cbigstr.py
char* glade_xml = "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<!-- Generated with glade 3.22.2 -->\n\
<interface>\n\
<requires lib=\"gtk+\" version=\"3.20\"/>\n\
<object class=\"GtkDialog\" id=\"dialog_list\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"title\" translatable=\"yes\">...</property>\n\
<property name=\"default_width\">500</property>\n\
<property name=\"default_height\">400</property>\n\
<property name=\"type_hint\">dialog</property>\n\
<child type=\"titlebar\">\n\
<placeholder/>\n\
</child>\n\
<child internal-child=\"vbox\">\n\
<object class=\"GtkBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"orientation\">vertical</property>\n\
<property name=\"spacing\">2</property>\n\
<child internal-child=\"action_area\">\n\
<object class=\"GtkButtonBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"layout_style\">end</property>\n\
<child>\n\
<placeholder/>\n\
</child>\n\
<child>\n\
<object class=\"GtkButton\" id=\"btn_dlg_close\">\n\
<property name=\"label\">gtk-close</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"receives_default\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<property name=\"always_show_image\">True</property>\n\
<signal name=\"clicked\" handler=\"on_btn_dlg_close_clicked\" swapped=\"no\"/>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">True</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">1</property>\n\
</packing>\n\
</child>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">False</property>\n\
<property name=\"fill\">False</property>\n\
<property name=\"position\">0</property>\n\
</packing>\n\
</child>\n\
<child>\n\
<object class=\"GtkScrolledWindow\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"shadow_type\">in</property>\n\
<child>\n\
<object class=\"GtkViewport\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<child>\n\
<object class=\"GtkListBox\" id=\"dlg_listbox\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"hexpand\">True</property>\n\
<property name=\"vexpand\">True</property>\n\
<signal name=\"row-activated\" handler=\"on_dlg_listbox_row_activated\" swapped=\"no\"/>\n\
</object>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">False</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">1</property>\n\
</packing>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
<object class=\"GtkMessageDialog\" id=\"dlg_messagebox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"type_hint\">dialog</property>\n\
<property name=\"message_type\">other</property>\n\
<child type=\"titlebar\">\n\
<placeholder/>\n\
</child>\n\
<child internal-child=\"vbox\">\n\
<object class=\"GtkBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"orientation\">vertical</property>\n\
<property name=\"spacing\">2</property>\n\
<child internal-child=\"action_area\">\n\
<object class=\"GtkButtonBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"homogeneous\">True</property>\n\
<property name=\"layout_style\">end</property>\n\
<child>\n\
<placeholder/>\n\
</child>\n\
<child>\n\
<object class=\"GtkButton\" id=\"btn_dlg_msg_close\">\n\
<property name=\"label\">gtk-close</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"receives_default\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<property name=\"always_show_image\">True</property>\n\
<signal name=\"clicked\" handler=\"on_btn_dlg_msg_close_clicked\" swapped=\"no\"/>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">True</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">1</property>\n\
</packing>\n\
</child>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">False</property>\n\
<property name=\"fill\">False</property>\n\
<property name=\"position\">0</property>\n\
</packing>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
<object class=\"GtkWindow\" id=\"window1\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"skip_taskbar_hint\">True</property>\n\
<signal name=\"destroy\" handler=\"on_window1_destroy\" swapped=\"no\"/>\n\
<signal name=\"key-press-event\" handler=\"on_window1_key_press_event\" swapped=\"no\"/>\n\
<child type=\"titlebar\">\n\
<placeholder/>\n\
</child>\n\
<child>\n\
<object class=\"GtkBox\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<child>\n\
<object class=\"GtkEntry\" id=\"entry\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"max_length\">1024</property>\n\
<property name=\"shadow_type\">none</property>\n\
<property name=\"placeholder_text\" translatable=\"yes\">type help for 'help'</property>\n\
<signal name=\"activate\" handler=\"on_entry_activate\" swapped=\"no\"/>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">True</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">0</property>\n\
</packing>\n\
</child>\n\
<child>\n\
<object class=\"GtkButton\" id=\"btn_entry\">\n\
<property name=\"label\" translatable=\"yes\">■</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"receives_default\">True</property>\n\
<signal name=\"clicked\" handler=\"on_btn_entry_clicked\" swapped=\"no\"/>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">False</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">1</property>\n\
</packing>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
</interface>\n\
\0";


// Global Pointers
GtkWidget *g_dlg_listbox;
GtkWidget *g_dialog_box;
GtkWidget *g_entry;
GtkWidget *g_label;
GtkWidget *g_messagebox;
GtkClipboard *g_clipboard;
GtkWidget *g_wnd;

// Global variables
char g_editor[128] = {0};  // user's editor executable name
char g_sea_engine[128] = {0};  // user's search engine string
int cmdline = 0;  // 1 is command line, 0 is GUI
int rsp = 0;
int g_win_level = 0;
char descq_path[128];
// action history buff
char g_last_entry[128] = {0};  // save user's last command only


// Add a seach text to hist.txt file
void write_history(char *text) {
    FILE *fh;
    char buf[80];
    char rec[500][BUFFER2];
    int count = 0;
    fh = open_for_read("data/hist.txt");
    while(1) {
        fgets(rec[count], BUFFER2, fh);
        if (feof(fh)) break;
        removen(rec[count++]);  // remove newline character
        if (count > 498) return;  // too many hist recs !!
    }
    fclose(fh);
    fh = open_for_write("data/hist.txt");
    fprintf(fh, "%s -- %s\n", text, today(buf));
    for(int x=0; x < count; x++) {
       fprintf(fh, "%s\n", rec[x]);
    }
    fclose(fh);
}


 //////////////////////////
// Display a message box /
void show_message(char pmsg[64], char smsg[64]) {
    gtk_message_dialog_set_markup (GTK_MESSAGE_DIALOG(g_messagebox), pmsg);
    gtk_message_dialog_format_secondary_markup(GTK_MESSAGE_DIALOG(g_messagebox), smsg, NULL);
    gtk_dialog_run(GTK_DIALOG(g_messagebox));
}

void commands(char *out_str) {
    /*  Continued parsing of request and
        Command-Line Section:
        descq was executed in a shell with
        arguments that were either a
        command or an internet search.
    */
    FILE *fh;
    char action[BUFFER2] = {0};
    char line[BUFFER2] = {0};
    char *ptr;
    int count = 0;
    int rsp = 0;

    /* open serv.txt and try to locate a commnad
       if not found will continue on to eventually search */
    fh = open_for_read("data/serv.txt");
    while(1) {
        fgets(line, BUFFER2, fh);
        if (feof(fh)) break;
        removen(line);  // remove newline character
        ptr = strchr(line, ',');  // get pointer to the first ','
        if (ptr) {
            *ptr = '\0';  // replace ',' with end of string \0 for line
            strcpy(action, ltrim(++ptr));  // copy trimmed string after ','
            if (startswith(line, out_str)) {  // find command in file
                if (startswith(action, "http")) {  // if action is a website
                    strcpy(line, "xdg-open ");
                    strcat(line, action);
                    system(line);
                } else {  // action is a local command
                    rsp = system(action);
                    if (rsp != 0) {
                        if (cmdline == 1) {  // command line NOT GUI
                            printf("Not Found in Serv.txt:\n%s\n", out_str);
                        } else {
                            show_message(out_str,"returned an error");
                        }
                    }
                }
                fclose(fh);
                return;
            }
        }
    }  // end while
    fclose(fh);
    // if (cmdline == 1) {
    //     printf("Not Found in Serv.txt:\n%s\n", out_str);
    // } else {
    //     show_message(out_str, "<b>Not Found in serv.txt</b>");
    // }

    if (out_str[1] == ':') {  // run a service query from serv.txt
        action[0] = out_str[0];
        action[1] = ',';
        action[2] = '\0';  // built key
        // Read the serv.txt file to find the correct line: letter:searchtext
        fh = open_for_read("data/serv.txt");
        while(1) {
            fgets(line, BUFFER2, fh);
            if (feof(fh)) break;
            if (startswith(line, action)) {
                write_history(out_str);
                removen(line);  // remove newline character
                count = fields(line, ",");   // myclib.h
                strcpy(line, "xdg-open ");  // build the command ...
                strcat(line, _fields[2]);  // should return count = 3 (0,1,2)
                // urlencode the search text
                strcpy(action, out_str+2);
                strcpy(action, urlencode(action));
                strcat(line, action);  // add search text to url query
                printf("%s\n", line);
                system(line);
                break;
            }
        }
        fclose(fh);

    } else if (equals(out_str, "list") && cmdline == 1) {
        system("cat data/serv.txt");

    } else {
        // It's an Internet Search using default browser
        write_history(out_str);
        strcpy(out_str, urlencode(out_str));
        strcpy(action, "xdg-open ");
        strcat(action, g_sea_engine);
        strcat(action, "\"");
        strcat(action, out_str);  // quotes needed incase of embedded quote
        strcat(action, "\"");
        strcat(action, " &");
        system(action);
    }
}


void change_win_level() {
    if (g_win_level == 1) {
        gtk_window_set_keep_above(GTK_WINDOW(g_wnd), false);
        g_win_level = 0;
    } else {
        gtk_window_set_keep_above(GTK_WINDOW(g_wnd), true);
        g_win_level = 1;
    }
}

void change_editor() {
    FILE *fh;
    fh = open_for_read("data/editor.txt");
    fgets(g_editor, 127, fh);
    removen(g_editor);
    fclose(fh);
}

void change_query_engine() {
    FILE *fh;
    fh = open_for_read("data/search.txt");
    fgets(g_sea_engine, 127, fh);
    removen(g_sea_engine);
    fclose(fh);
}

/*
 ___  ___       ___       __   __   __
|   \/   |     /   \     |  | |  \ |  |
|  \  /  |    /  ^  \    |  | |   \|  |
|  |\/|  |   /  /_\  \   |  | |  . `  |
|  |  |  |  /  _____  \  |  | |  |\   |
|__|  |__| /__/     \__\ |__| |__| \__|

*/

int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    FILE            *fh;
    int             w_top;
    int             w_left;
    int             w_width;
    int             w_height;
    int             w_decor;
    int             count = 0;
    char            line[BUFFER2];

    gtk_init(&argc, &argv);

    // Change to data directory: /home/USER/.config/descq/
    // the glade file and all of the data files are in this location
    // sprintf(descq_path, "%s/.config/descq", getenv("HOME"));
    strcpy(descq_path, "/usr/local/descq-data");
    chdir(descq_path);

    // load search engine query text
    change_query_engine();

    // Presence of args indicates a command line request
    if (argc > 1) {
        char cargs[128] = {0};
        int x = 0;
        for(x=1; x<argc-1; x++) {  // concat all the args
            strcat(cargs, argv[x]);
            strcat(cargs, " ");
        }
        strcat(cargs, argv[x]);  // no space after last arg
        cmdline = 1;

        commands(cargs);

        exit(0);  // back to command line
    }

    // load the layout and begin GUI operation

    //builder = gtk_builder_new();
    //gtk_builder_add_from_file (builder, "descq.glade", NULL);
    builder = gtk_builder_new_from_string (glade_xml, -1);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    gtk_builder_connect_signals(builder, NULL);

    g_wnd = window;  // to use in subroutines

    // set GtkWidget Pointers to their builder objects
    // g_ = GTK_WIDGET(gtk_builder_get_object(builder, "WIDGET_ID"));
    g_dlg_listbox = GTK_WIDGET(gtk_builder_get_object(builder, "dlg_listbox"));
    g_dialog_box = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_list"));
    g_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry"));
    g_clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    g_messagebox = GTK_WIDGET(gtk_builder_get_object(builder, "dlg_messagebox"));

    g_object_unref(builder);

    // load the window metrics
    fh = open_for_read("data/winmet.txt");
    fgets(line, 64, fh);
    fclose(fh);
    removen(line);  // remove new line character
    fields(line, ",");
    w_left      = atoi(_fields[0]);
    w_top       = atoi(_fields[1]);
    w_width     = atoi(_fields[2]);
    w_height    = atoi(_fields[3]);
    w_decor     = atoi(_fields[4]);

    gtk_widget_show(window);
    gtk_window_move(GTK_WINDOW(g_wnd), w_left, w_top);  // set metrics ...
    gtk_window_resize(GTK_WINDOW(g_wnd), w_width, w_height);
    // caption bar always off at startup
    gtk_window_set_decorated (GTK_WINDOW(g_wnd), w_decor);  // caption bar

    // count lines in urls.txt
    fh = open_for_read("data/urls.txt");
    while(1) {
        fgets(line, BUFFER2, fh);
        if (feof(fh)) break;
        count++;  // counting URLS in file
    }
    fclose(fh);
    if (count > MAXURLS) {
        show_message("<big>DescQ</big>", "Reached MAX URLS");
    }

    // window keep above other windows ("top" will toggle)
    // change_win_level();

    // load text editor name
    change_editor();

    gtk_main();

    return 0;
}

 /*_______________________________________________________________________________*/
/*______________________________END__OF__MAIN____________________________________*/


// called when window is closed
void on_window1_destroy()
{
    gtk_main_quit();
}


void displayListDlg(char * target) {
    GtkListBoxRow *g_row;
    FILE *fh;
    char line[BUFFER2];
    char filename[25];

    // remove any existing rows
    while (1) {
        g_row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(g_dlg_listbox), 0);
        if (g_row == NULL) break;
        gtk_widget_destroy (GTK_WIDGET(g_row));
    }
    // Set the target file and caption heading
    if (equals(target, "urls")) {
        strcpy(filename, "data/urls.txt");
        gtk_window_set_title(GTK_WINDOW(g_dialog_box), "Saved URL List");
    } else {
        if (equals(target, "serv")) {
            strcpy(filename, "data/serv.txt");
            gtk_window_set_title(GTK_WINDOW(g_dialog_box), "Your Commands");
        } else {
            strcpy(filename, "data/hist.txt");
            gtk_window_set_title(GTK_WINDOW(g_dialog_box), "Saved Search Queries");
        }
    }

    fh = open_for_read(filename);
    while(1) {
        fgets(line, BUFFER2, fh);
        if (feof(fh)) break;
        if (strstr(line, "-- SERVICES --")) break;  // only in serv.txt (hopefully!)
        removen(line);  // remove newline character
        g_label = gtk_label_new (line);
        gtk_label_set_xalign (GTK_LABEL(g_label), 0.0);
        gtk_list_box_insert(GTK_LIST_BOX(g_dlg_listbox), g_label, -1);
    }
    fclose(fh);
    //  Show the dialog box with the listbox
    gtk_widget_show_all(g_dialog_box);
    gtk_dialog_run(GTK_DIALOG(g_dialog_box));
    gtk_widget_hide (g_dialog_box);
}


void write_url(char *text) {  // Save url to urls.txt file and online file
    FILE *fh;
    char rec[500][BUFFER2];
    int count = 0;
    fh = open_for_read("data/urls.txt");
    while(1) {
        fgets(rec[count], BUFFER2, fh);
        if (feof(fh)) break;
        removen(rec[count++]);  // remove newline character
        if (count > 498) return;  // too many urls !!
    }
    fclose(fh);
    fh = open_for_write("data/urls.txt");
    fprintf(fh, "%s\n", text);
    for(int x=0; x < count; x++) {
       fprintf(fh, "%s\n", rec[x]);
    }
    fclose(fh);
}


// Save clipboard text contents to "clip.txt" file
void save_clipboard_to_file() {
    FILE *fh;
    char *cliptxt;

    cliptxt = (char *) malloc(5 * BUFFER3);  // 5MG

    if (!gtk_clipboard_wait_is_text_available(g_clipboard)) {
        return;
    }
    strcpy(cliptxt, gtk_clipboard_wait_for_text(g_clipboard));
    fh = open_for_append("data/clip.txt");
    fprintf(fh,"%s\n\n", cliptxt);
    fclose(fh);
    free(cliptxt);
}

// "Close" was clicked in messagebox
void on_btn_dlg_msg_close_clicked() {
    gtk_widget_hide(g_messagebox);
}


void process_entry(char *out_str) {
    FILE *fh;
    char action[BUFFER2] = {0};
    //gchar out_str[BUFFER2] = {0};
    int w_top;
    int w_left;
    int w_width;
    int w_height;


    strcpy(g_last_entry, out_str);  // copy command into g_last_entry

    if (equalsignorecase(out_str, "list")) {        // list urls
        displayListDlg("urls");

    } else if (equalsignorecase(out_str, "hist")) { // list history
        displayListDlg("hist");

    } else if (equalsignorecase(out_str, "serv")) { // list commands from serv.txt
        displayListDlg("serv");

    } else if (startswith(out_str, "http")) {       // saves URL to urls.txt
        write_url(out_str);

    } else if (equals(out_str, "sc")) {             // Save clipboard to clip.txt
        save_clipboard_to_file();

    } else if (equals(out_str, "es")) {             // edit serv.txt
        strcpy(action, EDITOR);
        strcat(action, " data/serv.txt &");
        system(action);

    } else if (equals(out_str, "eh")) {        // edit hist.txt
        strcpy(action, EDITOR);
        strcat(action, " data/hist.txt &");
        system(action);

    } else if (equals(out_str, "ec")) {        // edit clip.txt
        strcpy(action, EDITOR);
        strcat(action, " data/clip.txt &");
        system(action);

    } else if (equals(out_str, "eu")) {        // edit urls.txt
        strcpy(action, EDITOR);
        strcat(action, " data/urls.txt &");
        system(action);

    } else if (equalsignorecase(out_str, "help") || (equalsignorecase(out_str, "about"))) {  // edit help.txt
        strcpy(action, EDITOR);
        strcat(action, " data/help.txt &");
        int rsp = system(action);
        if (rsp != 0) {
            show_message("<big>text editor</big>", "is not found.");
        }

    } else if (equals(out_str, "ee")) {        // edit editor.txt
        strcpy(action, EDITOR);
        strcat(action, " data/editor.txt");
        system(action);
        change_editor();

    } else if (equals(out_str, "eq")) {         // edit editor.txt
        strcpy(action, EDITOR);
        strcat(action, " data/search.txt");
        system(action);
        change_query_engine();

    } else if (equals(out_str, "top")) {        // toggle window Z-order
        change_win_level();

    } else if (equals(out_str, "x")) {         // exit program
        on_window1_destroy();

    } else if (equals(out_str, "cap")) {        // toggle window caption bar
        if (gtk_window_get_decorated(GTK_WINDOW(g_wnd))) {
            gtk_window_set_decorated (GTK_WINDOW(g_wnd), 0);
        } else {
            gtk_window_set_decorated (GTK_WINDOW(g_wnd), 1);
        }

    } else if (equalsignorecase(out_str, "winset")) {        // save window position
        gtk_window_get_position (GTK_WINDOW(g_wnd), &w_left, &w_top);
        gtk_window_get_size (GTK_WINDOW(g_wnd), &w_width, &w_height);
        fh = open_for_write("data/winmet.txt");
        if (gtk_window_get_decorated(GTK_WINDOW(g_wnd))) {
            fprintf(fh, "%d,%d,%d,%d,1\n", w_left, w_top, w_width, w_height);
        } else {
            fprintf(fh, "%d,%d,%d,%d,0\n", w_left, w_top, w_width, w_height);
        }
        fclose(fh);

    } else if (charinx("$@>", out_str[0]) > -1) {  // run an app directly
        strcpy(action, out_str+1);
        strcat(action, " &");
        //printf("\n----> %d %s\n", charinx("$@>", out_str[0]), out_str+1);
        system(action);

    } else {  // continue on to check for other possible commands

        commands(out_str);  // these actions are shared by the command line process
    }
}


void on_entry_activate(GtkEntry *entry) {
    /*
    The user has typed something into the entry field and hit Enter.
    (or might have clicked the button instead of hitting Enter)
    Determine if it is a:
    1. descq command (e.g. list ...)
    2. alias from the serv.txt file
    3. Internet search text
    */
    gchar out_str[BUFFER2] = {0};
    int cnt = 0;
    int inx = 0;

    sprintf(out_str, "%s", gtk_entry_get_text(entry));
    gtk_entry_set_text(entry, "");  // clear the entry widget text
    trim(out_str);
    if (equals(out_str, "")) {
        return;
    }
    // User can enter multiple commands/searches delimited by "|"
    cnt = fields(out_str, "|");

    for(inx = 0; inx < cnt; inx++) {
        process_entry(_fields[inx]);
    }
}


void on_dlg_listbox_row_activated(GtkListBox *oList, GtkListBoxRow *oRow) {
    GtkWidget *bin;
    char listdata[BUFFER2];
    char *ptr;  // pointer used with listdata
    char url[BUFFER2];

    bin = gtk_bin_get_child(GTK_BIN(oRow));
    strcpy(listdata, gtk_label_get_text(GTK_LABEL(bin)));
    gtk_clipboard_set_text(g_clipboard, listdata, -1);
    if (startswith(listdata, "http")) {  // LIST item
        strcpy(url, "xdg-open ");
        strcat(url, listdata);
        strcat(url, " &");  // do not wait for system to finish
        system(url);
        gtk_widget_hide (g_dialog_box);
    } else {  // SERV item
        if (ptr = strchr(listdata, ',')) {
            *ptr = '\0';  // replace ',' with end of string \0 for line
            commands(listdata);
        } else {  // HIST item
            listdata[strlen(listdata) - 14] = '\0';  // cut off the date
            printf("%s\n", listdata);
            if (listdata[1] == ':') {
                gtk_entry_set_text(GTK_ENTRY(g_entry), listdata);
                on_entry_activate(GTK_ENTRY(g_entry));
            } else {
                strcpy(listdata, urlencode(listdata));
                strcpy(url, "xdg-open ");
                strcat(url, g_sea_engine);
                strcat(url, "\"");
                strcat(url, listdata);  // quotes necessary incase of embeded quote
                strcat(url, "\"");
                system(url);
            }
        }
    }
}

 // Execute the entry text,
// or if entry is empty save to clipboard.
void on_btn_entry_clicked() {
    gchar out_str[BUFFER2] = {0};
    sprintf(out_str, "%s", gtk_entry_get_text(GTK_ENTRY(g_entry)));
    trim(out_str);  // stringalt.h
    if (equals(out_str, "")) {  // Try to save clipboard contents to clip.txt
        // when entry field is empty.
        save_clipboard_to_file();
    } else {
        on_entry_activate(GTK_ENTRY(g_entry));  // else treat it as Enter key pressed
    }
}

_Bool on_window1_key_press_event(GtkWidget *w, GdkEvent *e) {
    guint keyval;

    gdk_event_get_keyval (e, &keyval);
    //printf("%d\n", keyval);

    if (keyval == 65364) {  // DOWN ARROW
        gtk_entry_set_text(GTK_ENTRY(g_entry), g_last_entry);
        return TRUE;
    }

    if (keyval == 65362) {  // UP ARROW
        gtk_entry_set_text(GTK_ENTRY(g_entry), g_last_entry);
        return TRUE;
    }

    if (keyval == 65307) {  // Escape key
        gtk_entry_set_text(GTK_ENTRY(g_entry), "");
        return TRUE;
    }

    return FALSE;
}


// Exit the program
void on_btn_dlg_close_clicked() {
    gtk_widget_hide(g_dialog_box);
}
