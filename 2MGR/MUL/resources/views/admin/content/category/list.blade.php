@extends('admin.pages.main')

@section('content')
    <h4 class="ml-lg-3 mt-lg-3">Přehled kategorií</h4>
    @include('flash::message')
    <table id="example" class="table table-striped table-bordered">
        <thead>
        <tr>
            <th>Název</th>
            <th>Url</th>
            <th>Pořadí</th>
            <th>Počet videozáznamů</th>
            <th>Vytvořeno</th>
            <th>Akce</th>
        </tr>
        </thead>
        <tbody>
        @foreach($categories as $category)
            <tr>
                <td>{{ $category->title }}</td>
                <td>{{ $category->url }}</td>
                <td>{{ $category->sort }}</td>
                <td>{{ $category->videos_count }}</td>
                <td>{{ $category->created_at->format('j.m.Y H:i') }}</td>
                <td style="text-align: center;">
                    <a class="p-1" title="Upravit" href="{{ route('admin.categories_edit', $category->url) }}">
                        <i class="fa fa-pencil-square-o" aria-hidden="true"></i>
                    </a>
                    <a class="p-1" title="Smazat" href="{{ route('admin.categories_delete', $category->url) }}" data-toggle="modal" data-target="#removeCategoryModal_{{ $category->id }}">
                        <i class="fa fa-trash" aria-hidden="true"></i>
                    </a>
                    {{-- Remove category confirmation window --}}
                    <div class="modal fade" id="removeCategoryModal_{{ $category->id }}" tabindex="-1" role="dialog" aria-hidden="true">
                        <div class="modal-dialog" role="document">
                            <div class="modal-content">
                                <div class="modal-header">
                                    <h5 class="modal-title" id="exampleModalLabel">Opravdu chcete smazat kategorii <strong>{{ $category->title }}</strong>?</h5>
                                    <button class="close" type="button" data-dismiss="modal" aria-label="Close">
                                        <span aria-hidden="true">×</span>
                                    </button>
                                </div>
                                <div class="modal-body">@if($category->videos_count > 0) Kategorie obsahuje některé videozáznamy. Smazáním kategorie dojde ke smazání všech videozáznamů, které patří do této kategorie. Opravdu chcete kategorii smazat? @else Kliknutím na tlačítko "smazat" smažete vybranou kategorii. @endif</div>
                                <div class="modal-footer">
                                    <button class="btn btn-secondary" type="button" data-dismiss="modal">Zrušit</button>
                                    <a class="btn btn-primary" href="{{ route('admin.categories_delete', $category->url) }}">Smazat</a>
                                </div>
                            </div>
                        </div>
                    </div>
                    {{-- End remove category confirmation window --}}
                </td>
            </tr>
        @endforeach
        </tbody>
    </table>
    <a class="btn btn-primary btn-sm ml-lg-3 mt-3 mb-5" href="{{ route('admin.categories_create') }}" role="button"><i class="fa fa-pencil-square-o position-left"></i> Vytvořit kategorii</a>
@endsection

@push('script')
    <script>
        $(document).ready(function() {
            $('#example').DataTable({
                "paging": false,
                "columnDefs": [
                    { "orderable": false, "targets": 5 }
                ],
                "order": [[ 2, "asc" ]],
                "language": {
                    "lengthMenu": "Zobrazit _MENU_ záznamů na stránku",
                    "zeroRecords": "Žádné záznamy k zobrazení",
                    "info": "Strana _PAGE_ z _PAGES_",
                    "infoEmpty": "",
                    "infoFiltered": "(filtrováno z celkového počtu _MAX_ záznamů)",
                    "search" : "Hledat",
                    "paginate": {
                        "previous": "Předchozí",
                        "next" : "Další"
                    }
                }
            });
        } );
    </script>
@endpush