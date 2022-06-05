/* descq.c  Desk-Queue
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
#include <myc.h>
#include <mynet.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#define EDITOR g_editor
#define ONEKB 1024
#define TWOKB 2048
#define ONEMB 1000000
#define MAXURLS 200
#define HIST_LIMIT 10
#define HIST_SIZE 1024

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

// entry history
list hist;
int inx = HIST_LIMIT - 1;;

// Add a seach text to hist.txt file
void write_history(char *text) {
    FILE *fh;
    char rec[1000][TWOKB];
    int count = 0;
    fh = open_for_read("data/hist.txt");
    while(1) {
        fgets(rec[count], TWOKB, fh);
        if (feof(fh)) break;
        chomp(rec[count++]);  // remove newline character
        if (count > 990)
            ERRMSG(-1, true, "Too many history entries!");
    }
    fclose(fh);
    fh = open_for_write("data/hist.txt");
    fprintf(fh, "%s -- %s\n", text, date("%F"));
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
    char action[TWOKB] = {0};
    char line[TWOKB] = {0};
    char *ptr;
    int count = 0;
    int rsp = 0;
    /* open serv.txt and try to locate a commnad
       if not found will continue on to eventually search */
    fh = open_for_read("data/serv.txt");
    while(1) {
        fgets(line, TWOKB, fh);
        if (feof(fh)) break;
        if (startswith(line, "#")) {
            continue;
        }
        chomp(line);  // remove newline character
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

    if (out_str[1] == ':') {  // run a service query from serv.txt
        action[0] = out_str[0];
        action[1] = ',';
        action[2] = '\0';  // built key
        // Read the serv.txt file to find the correct line: letter:searchtext
        fh = open_for_read("data/serv.txt");
        while(1) {
            fgets(line, TWOKB, fh);
            if (feof(fh)) break;
            if (startswith(line, action)) {
                write_history(out_str);
                chomp(line);  // remove newline
                list vars = list_def(3, 1024);
                list_split(vars, line, ",");
                strcpy(line, "xdg-open ");     // build the command ...
                strcat(line, vars.item[2]); // should return count = 3 (0,1,2)
                list_del(vars);
                // urlencode the search text
                strcpy(action, out_str+2);
                char *linecoded = malloc(TWOKB);
                strcpy(action, urlencode(linecoded, action));
                free(linecoded);
                strcat(line, action);  // add search text to url query
                //printf("%s\n", line);
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
        char *linecoded = malloc(TWOKB);
        strcpy(out_str, urlencode(linecoded, out_str));
        free(linecoded);
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
    chomp(g_editor);
    fclose(fh);
}

void change_query_engine() {
    FILE *fh;
    fh = open_for_read("data/search.txt");
    fgets(g_sea_engine, 127, fh);
    chomp(g_sea_engine);
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
    char            line[TWOKB];

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
    chomp(line);  // remove new line character
    list vals = list_def(5, 16);
    list_split(vals, line, ",");
    w_left      = atoi(vals.item[0]);
    w_top       = atoi(vals.item[1]);
    w_width     = atoi(vals.item[2]);
    w_height    = atoi(vals.item[3]);
    w_decor     = atoi(vals.item[4]);
    list_del(vals);

    gtk_widget_show(window);
    gtk_window_move(GTK_WINDOW(g_wnd), w_left, w_top);  // set metrics ...
    gtk_window_resize(GTK_WINDOW(g_wnd), w_width, w_height);
    // caption bar always off at startup
    gtk_window_set_decorated (GTK_WINDOW(g_wnd), w_decor);  // caption bar

    // count lines in urls.txt
    fh = open_for_read("data/urls.txt");
    while(1) {
        fgets(line, TWOKB, fh);
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

    // load entry history from file
    hist = list_def(HIST_LIMIT, HIST_SIZE);
    if(file_exists("ehist.txt"))
        list_io(hist, "ehist.txt", 'r');

    gtk_main();

    return 0;
}

 /*_______________________________________________________________________________*/
/*______________________________END__OF__MAIN____________________________________*/


// called when window is closed
void on_window1_destroy() {
    list_io(hist, "ehist.txt", 'w');
    list_del(hist);
    gtk_main_quit();
}

void on_btn_dlg_close_clicked() {
    gtk_widget_hide(g_dialog_box);
}

void displayListDlg(char * target) {
    GtkListBoxRow *g_row;
    FILE *fh;
    char line[TWOKB];
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
        fgets(line, TWOKB, fh);
        if (feof(fh)) break;
        if (strstr(line, "-- SERVICES --")) break;  // only in serv.txt (hopefully!)
        chomp(line);  // removes newline character
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

char *get_page_title(char *buf, char *url) {
    const size_t bufsize = 1000000; // buffer to hold page html content
    // use 'webpage' to get the pages <title> text
    string pagebuf = string_def(bufsize, '\0');
    if (!webpage(pagebuf.value, bufsize, url)) {
      ERRMSG(-1, false, "get_page_title: 'webpage' failure\n");
      string_del(pagebuf);
      return url;
    } else {
      // puts(pagebuf.value);
      int p = between(buf, pagebuf.value, "<title>", "</title>", 0);
      if(p == -1) {
        ERRMSG(-1, false, "get_page_title: could not find title tag");
        string_del(pagebuf);
        return url;
      }
    }
    string_del(pagebuf);
    return buf;
}

/*  WRITE TO URL FILE
    save the url with its title
    into urls.txt file
*/
void write_url(char *text) {  // Save url to urls.txt file
    FILE *fh;
    char rec[1000][TWOKB];
    int count = 0;
    char title[1024] = {'\0'};
    const size_t bufsize = 1000000; // buffer to hold page html content
    fh = open_for_read("data/urls.txt");
    while(1) {
        fgets(rec[count], TWOKB, fh);
        if (feof(fh)) break;
        chomp(rec[count++]);  // remove newline character
        if (count > 990)
            ERRMSG(-1, true, "Too many URLs for urls.txt!");
    }
    fclose(fh);

    // use mynet.h webpage to get the title
    get_page_title(title, text);
    deletechar(title, title, "\n", 0, 0);
    if(strlen(title) > 5)
        concat(title, " <=> ", text, END);
    else
        strcpy(title, text);

    fh = open_for_write("data/urls.txt");
    fprintf(fh, "%s\n", title);
    for(int x=0; x < count; x++) {
       fprintf(fh, "%s\n", rec[x]);
    }
    fclose(fh);
}

// Save clipboard text contents to "clip.txt" file
void save_clipboard_to_file() {
    FILE *fh;
    char *cliptxt;

    cliptxt = (char *) malloc(2 * ONEMB);  // 1 MGB

    if (!gtk_clipboard_wait_is_text_available(g_clipboard)) {
        return;
    }
    strcpy(cliptxt, gtk_clipboard_wait_for_text(g_clipboard));
    if (startswith(cliptxt, "http")) {
        write_url(cliptxt);
        free(cliptxt);
    } else {
        fh = open_for_append("data/clip.txt");
        fprintf(fh,"%s\n\n", cliptxt);
        fclose(fh);
        free(cliptxt);
    }
}

// "Close" was clicked in messagebox
void on_btn_dlg_msg_close_clicked() {
    gtk_widget_hide(g_messagebox);
}

const char * get_bc_result(const char * expr) {
    FILE *fp;
    static char path[1024];
    char bcxpr[1024] = {'\0'};

    // format the pipe to bc
    strcpy(bcxpr, "echo \"scale=4;");
    strcat(bcxpr, expr);
    strcat(bcxpr, "\" | bc");

    /* Open the command for reading. */
    fp = popen(bcxpr, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        return "fail";
    }
    /* Read the output a line at a time (output it.) */
    while (fgets(path, sizeof(path), fp) != NULL) {
        // printf("%s", path);
    }

    /* close */
    pclose(fp);
    return chomp(path);
}


void process_entry(char *out_str) {
    FILE *fh;
    char action[TWOKB] = {0};
    //gchar out_str[TWOKB] = {0};
    int w_top;
    int w_left;
    int w_width;
    int w_height;

    // store for entry history
    if(!equals(out_str, "x")) {
        if(list_find(hist, out_str) == -1) { // not in list yet so add it
            list_inject(hist, out_str, 0);
            inx = HIST_LIMIT - 1;
            list_io(hist, "ehist.txt", 'w'); // not getting writen on destroy...
        }
    }

    if (equalsignore(out_str, "list")) {        // list urls
        displayListDlg("urls");

    } else if (equalsignore(out_str, "hist")) { // list history
        displayListDlg("hist");

    } else if (equalsignore(out_str, "serv") || (equals(out_str, "?"))) { // list commands from serv.txt
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

    } else if (equalsignore(out_str, "help") || (equalsignore(out_str, "about"))) {  // edit help.txt
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

    } else if (equalsignore(out_str, "winset")) {        // save window position
        gtk_window_get_position (GTK_WINDOW(g_wnd), &w_left, &w_top);
        gtk_window_get_size (GTK_WINDOW(g_wnd), &w_width, &w_height);
        fh = open_for_write("data/winmet.txt");
        if (gtk_window_get_decorated(GTK_WINDOW(g_wnd))) {
            fprintf(fh, "%d,%d,%d,%d,1\n", w_left, w_top, w_width, w_height);
        } else {
            fprintf(fh, "%d,%d,%d,%d,0\n", w_left, w_top, w_width, w_height);
        }
        fclose(fh);

    } else if (charat("$@>", out_str[0]) > -1) {  // run an app directly
        strcpy(action, out_str+1);
        strcat(action, " &");
        //printf("\n----> %d %s\n", charinx("$@>", out_str[0]), out_str+1);
        system(action);

    } else if (out_str[0] == '=') {  // evaluate this math expression
        strcpy(action, out_str+1);
        const char * res = get_bc_result(action);
        gtk_entry_set_text(GTK_ENTRY(g_entry), res);

    } else {  // continue on to check for other possible commands
        commands(out_str);  // these actions are shared by the command line process
    }
}


void on_entry_activate(GtkEntry *entry) {
    /*
    The user has typed something into the entry field and hit Enter.
    (or might have clicked the button instead of hitting Enter)
    Determine if it is a:
    1. bcb command (e.g. list ...)
    2. alias from the serv.txt file
    3. Internet search text
    */
    gchar out_str[TWOKB] = {0};
    int cnt = 0;
    int inx = 0;

    sprintf(out_str, "%s", gtk_entry_get_text(entry));
    gtk_entry_set_text(entry, "");  // clear the entry widget text
    trim(out_str);
    if (equals(out_str, "")) {
        return;
    }
    // User can enter multiple commands/searches delimited by "|"
    list coms = list_def(20, TWOKB);
    cnt = list_split(coms, out_str, "|");
    for(inx = 0; inx < cnt; inx++) {
        process_entry(coms.item[inx]);
    }
    list_del(coms);
}


void on_dlg_listbox_row_activated(GtkListBox *oList, GtkListBoxRow *oRow) {
    GtkWidget *bin;
    char listdata[TWOKB] = {'\0'};
    char *ptr;  // pointer used with listdata
    char url[TWOKB] = {'\0'};

    bin = gtk_bin_get_child(GTK_BIN(oRow));
    strcpy(listdata, gtk_label_get_text(GTK_LABEL(bin)));
    gtk_clipboard_set_text(g_clipboard, listdata, -1);
    // LIST (URLS) item
    if ((indexof(listdata, " <=> http") >= 0) || (startswith(listdata, "http"))) {
        if (ptr = strstr(listdata, " <=> ")) {
            ptr += 5;
        } else {
            ptr = listdata;
        }
        concat(url, "xdg-open ", ptr, " &", END);
        system(url);
    } else {  // SERV item
        if (ptr = strchr(listdata, ',')) {
            *ptr = '\0';  // replace ',' with end of string \0 for line
            commands(listdata);
        } else {  // HIST item
            listdata[strlen(listdata) - 14] = '\0';  // cut off the date
            if (listdata[1] == ':') {
                gtk_entry_set_text(GTK_ENTRY(g_entry), listdata);
                on_entry_activate(GTK_ENTRY(g_entry));
            } else {
                char *linecoded = malloc(TWOKB);
                strcpy(listdata, urlencode(linecoded, listdata));
                free(linecoded);
                concat(url, "xdg-open ", g_sea_engine, "\"", listdata, "\"", END);
                system(url);
            }
        }
    }
    gtk_widget_hide (g_dialog_box);
}

 // Execute the entry text,
// or if entry is empty save to clipboard.
void on_btn_entry_clicked() {
    gchar out_str[TWOKB] = {0};
    sprintf(out_str, "%s", gtk_entry_get_text(GTK_ENTRY(g_entry)));
    trim(out_str);  // stringalt.h
    if (equals(out_str, "")) {  // when entry field is empty.
        save_clipboard_to_file();  // Save clipboard contents to clip.txt
    } else {
        on_entry_activate(GTK_ENTRY(g_entry));  // else treat it as Enter key pressed
    }
}

_Bool on_window1_key_press_event(GtkWidget *w, GdkEvent *e) {
    guint keyval;

    // change entry history index from arrow keys
    int move_inx(int direction, int inx) {
        if(direction == 0) { // (0) arrow UP "backwards"
            inx++;
            if(inx >= HIST_LIMIT)
                inx = 0;
        } else { // (1) arrow DOWN "forwards"
            inx--;
            if(inx < 0)
                inx = 0; // Stop at most current
        }
        return inx;
    }

    gdk_event_get_keyval (e, &keyval);
    //printf("%d\n", keyval);

    if (keyval == 65289) {  // TAB KEY PRESSED - Process entry!
        on_entry_activate(GTK_ENTRY(g_entry));
        return TRUE;
    }

    if (keyval == 65364) {  // DOWN ARROW
        inx = move_inx(1, inx);
        gtk_entry_set_text(GTK_ENTRY(g_entry), hist.item[inx]);
        return TRUE;
    }

    if (keyval == 65362) {  // UP ARROW
        inx = move_inx(0, inx);
        gtk_entry_set_text(GTK_ENTRY(g_entry), hist.item[inx]);
        return TRUE;
    }

    if (keyval == 65307) {  // Escape key
        gtk_entry_set_text(GTK_ENTRY(g_entry), "");
        return TRUE;
    }

    return FALSE;
}
