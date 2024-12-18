/**
 * @brief handle CTRL-C input for parent process
 */
void handleRootCTRLC() {
    signal(SIGINT, handleRootCTRLC);
}

/**
 * @brief handle CTRL-C input for child process
 */
void handleChildCTRLC() {
    signal(SIGINT, handleRootCTRLC);
    exit(0);
}