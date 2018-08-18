@extends('admin.pages.main')

@section('content')
    <h4 class="ml-lg-3 mt-lg-3 mb-5">Přehled videozáznamů</h4>
    @include('flash::message')
    <table id="example" class="table table-striped table-bordered">
        <thead>
        <tr>
            <th>Název</th>
            <th>Url</th>
            <th>Kategorie</th>
            <th>Formát</th>
            <th>Trvání</th>
            <th>Velikost</th>
            <th>Počet shlédnutí</th>
            <th>Vytvořeno</th>
            <th>Akce</th>
        </tr>
        </thead>
        <tbody>
        @foreach($videos as $video)
            <tr>
                <td>{{ $video->title }}</td>
                <td>{{ $video->url }}</td>
                <td>{{ $video->category->title }}</td>
                <td>{{ strtolower($video->file_format) }}</td>
                <td>{{ $video->file_duration ?? 'Nedostupné' }}</td>
                <td>{{ $video->file_size_formatted }}</td>
                <td>{{ $video->views_count }}</td>
                <td>{{ $video->created_at->format('j.m.Y H:i') }}</td>
                <td style="text-align: center;">
                    <a class="p-1" title="Upravit" href="{{ route('admin.videos_edit', $video->url) }}">
                        <i class="fa fa-pencil-square-o" aria-hidden="true"></i>
                    </a>
                    <a class="p-1" title="Smazat" href="{{ route('admin.videos_delete', $video->url) }}" data-toggle="modal" data-target="#removeVideoModal_{{ $video->id }}">
                        <i class="fa fa-trash" aria-hidden="true"></i>
                    </a>
                    {{-- Remove video confirmation window --}}
                    <div class="modal fade" id="removeVideoModal_{{ $video->id }}" tabindex="-1" role="dialog" aria-hidden="true">
                        <div class="modal-dialog" role="document">
                            <div class="modal-content">
                                <div class="modal-header">
                                    <h5 class="modal-title" id="exampleModalLabel">Opravdu chcete smazat videozáznam <strong>{{ $video->title }}</strong>?</h5>
                                    <button class="close" type="button" data-dismiss="modal" aria-label="Close">
                                        <span aria-hidden="true">×</span>
                                    </button>
                                </div>
                                <div class="modal-body">Kliknutím na tlačítko "smazat" smažete vybraný videozáznam.</div>
                                <div class="modal-footer">
                                    <button class="btn btn-secondary" type="button" data-dismiss="modal">Zrušit</button>
                                    <a class="btn btn-primary" href="{{ route('admin.videos_delete', $video->url) }}">Smazat</a>
                                </div>
                            </div>
                        </div>
                    </div>
                    {{-- End remove video confirmation window --}}
                </td>
            </tr>
        @endforeach
        </tbody>
    </table>
    <a class="btn btn-primary btn-sm ml-lg-3 mb-5" href="{{ route('admin.videos_create') }}" role="button"><i class="fa fa-pencil-square-o position-left"></i> Vložit videozáznam</a>
@endsection

@push('script')
    <script>
        $(document).ready(function() {
            $('#example').DataTable({
                "order": [[ 0, "asc" ]],
                "columnDefs": [
                    { "orderable": false, "targets": 8 }
                ],
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