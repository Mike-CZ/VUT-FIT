<div class="admin-home">
    <?php if(isset($admin_active)): ?>
        <div>
            <a class="admin-button" href="/admin-insert">
                <div>
                    Vložit otázku
                </div>
            </a>
        </div>
        <div>
            <a class="admin-button" href="/admin-delete">
                <div>
                    Smazat otázku
                </div>
            </a>
        </div>
        <br />
        <div>
            <a class="admin-button" href="/admin-logout">
                <div>
                    Odhlásit se
                </div>
            </a>
        </div>

    <?php else: ?>
    Chyba při zpracování požadavku
    <?php endif; ?>
</div>