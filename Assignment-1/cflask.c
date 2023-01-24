#include "functionslist.h"

// This function will be called by civetweb on every new request.
static int begin_request_handler(struct mg_connection *conn)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);

    int ind = get_index(request_info->uri);
    char content[100];
    if (ind == -1) {
        // Prepare the message we're going to send
        int content_length = snprintf(content, sizeof(content),
                                  "Sorry, requested url/function not found:)");
        // Send HTTP reply to the client
        mg_printf(conn,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
              "\r\n"
              "%s",
              content_length, content);
        return 1;
    }
    else{
        char *params = (char*) request_info->query_string;
        char *return_value = functionList[ind]((void*)params);

        // Prepare the message we're going to send
        int content_length = snprintf(content, sizeof(content),
                                  "%s", return_value);
        // Send HTTP reply to the client
        mg_printf(conn,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
              "\r\n"
              "%s",
              content_length, content);

        // Returning non-zero tells civetweb that our function has replied to
        // the client, and civetweb should not send client any more data.
        return 1;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s port num_threads\n", argv[0]);
        return 1;
    }

    char *port = argv[1];
    char *num_threads = argv[2];
    struct mg_context *ctx;
    struct mg_callbacks callbacks;

    // List of options. Last element must be NULL.
    const char *options[] = {"listening_ports", port, "num_threads", num_threads, NULL};

    // Prepare callbacks structure. We have only one callback, the rest are NULL.
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.begin_request = begin_request_handler;

    // Start the web server.
    ctx = mg_start(&callbacks, NULL, options);

    printf("Hey there! I'm a cflask server.\n");
    printf("Press enter to exit.\n");

    // Wait until user hits "enter". Server is running in separate thread.
    // Navigating to http://localhost:port will invoke begin_request_handler().
    getchar();

    // Stop the server.
    mg_stop(ctx);
    printf("Server stopped. Bye!!\n");

    return EXIT_SUCCESS;
}
